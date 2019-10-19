#include "hydra.h"

#ifdef OPENGLD
  #include <GL/glut.h>
#else
  #include <GL/freeglut.h>
#endif

#ifdef _WIN32
  #include <windows.h>
  //Shitty windows system
  #define popen _popen
  #define pclose _pclose
#else
  #include <unistd.h>
#endif

#include <iostream>

#include <string.h>

#include <time.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

namespace hydra
{

float rotation = 0;
float direction = 0.5f;

std::shared_ptr<Environment> Environment::instance;
RegisterAssociation Environment::registrations[256];

void Environment::clear()
{
  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    instance->entities.at(i)->kill();
  }

  instance->downKeys.clear();
  Mouse::clearButtons();
}

void Environment::setupPaths(char *argv0)
{
  FILE* process = NULL;
  std::string command;
  char buffer[512] = {0};
  
  command = "cd `dirname \\`which " + std::string(argv0) + "\\``; cd ..; pwd | tr -d '\n'";
  process = popen(command.c_str(), "r");
  
  if(process == NULL)
  {
    throw Exception("Failed to open child process");
  }

  fgets(buffer, 512, process);
  pclose(process);
  prefix = buffer;

  std::cout << "Prefix: " << prefix << std::endl;

  command = "basename " + std::string(argv0) + " | tr -d '\n'";
  process = popen(command.c_str(), "r");

  if(process == NULL)
  {
    throw Exception("Failed to open child process");
  }

  fgets(buffer, 512, process);
  pclose(process);
  std::string name = buffer;
  assetsDirectory = prefix + "/share/" + name;

  //TODO TEMPORY WINDOWS WORK-AROUND
#if _WIN32
  assetsDirectory = "share/tmgg/";
#endif

  std::cout << "Assets: " << assetsDirectory << std::endl;
}

std::sr1::shared_ptr<rend::Context> Environment::getContext()
{
  return instance->graphics;
}

void Environment::initializePre(int argc, char *argv[])
{
  instance = std::make_shared<Environment>();

  instance->setupPaths(argv[0]);

  instance->screenWidth = 1024;
  instance->screenHeight = 768;

  instance->graphics = rend::Context::initialize(argc, argv);

  instance->openAudio();

  glutDisplayFunc(display);
  glutIdleFunc(idle);

  glutKeyboardFunc(Keyboard::keyboard);
  glutMotionFunc(Mouse::motion);
  glutPassiveMotionFunc(Mouse::motion);
  glutMouseFunc(Mouse::mouse);

  // TODO
  //glutSpecialFunc(Keyboard::keyboardSpecial);
  //glutSpecialUpFunc(Keyboard::keyboardSpecialUp);

  glutKeyboardUpFunc(Keyboard::keyboardUp);

  // TODO
  //glutReshapeFunc(reshape);

  //Vector4 col = Camera::getClearColor();
  //glClearColor(col.x, col.y, col.z, col.w);
  //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  Camera::setClearColor(Vector4(0, 0, 0.2f, 1.0f));

#ifdef USE_GLUTEN
  gnEnable(GN_TEXTURE_2D);
#else
  glEnable(GL_TEXTURE_2D);
#endif
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

/*
  std::shared_ptr<World> world = World::load(path);
  if(world.get() == NULL)
  {
    std::cerr << "Err" << std::endl;
    throw std::exception();
  }
  world->populate();
*/

  Gui::initialize();
}

void Environment::initializePost()
{
  //spawn(entryComponent, Vector3(), Vector3());

  instance->startTime = time(NULL);

  glutFullScreen();
  glutMainLoop();

  instance->closeAudio();

  instance.reset();

#ifdef OPENGLD
  glutdCleanup();
#endif
}

void Environment::openAudio()
{
  // Open up the OpenAL device
  device = alcOpenDevice(NULL);

  if(device == NULL)
  {
    std::cout << "Failed to open device" << std::endl;
    return;
    throw std::exception();
  }

  // Create audio context
  context = alcCreateContext(device, NULL);

  if(context == NULL)
  {
    alcCloseDevice(device);
    std::cout << "Failed to create context" << std::endl;
    throw std::exception();
  }

  // Set as current context
  if(!alcMakeContextCurrent(context))
  {
    alcDestroyContext(context);
    alcCloseDevice(device);
    std::cout << "Failed to make context current" << std::endl;
    throw std::exception();
  }
}

void Environment::stopAudio()
{
  if(!instance->device) return;

  for(size_t asi = 0; asi < instance->audioSources.size(); asi++)
  {
    ALuint& as = instance->audioSources.at(asi);
    alDeleteSources(1, &as);
  }

  instance->audioSources.clear();
}

void Environment::closeAudio()
{
  if(!device) return;

  for(size_t asi = 0; asi < audioSources.size(); asi++)
  {
    ALuint& as = audioSources.at(asi);

    while(true)
    {
      ALint state = 0;

      alGetSourcei(as, AL_SOURCE_STATE, &state);

      if(state == AL_STOPPED)
      {
        alDeleteSources(1, &as);
        break;
      }

#ifdef _WIN32
      Sleep(1000);
#else
      sleep(1);
#endif
    }
  }

  audioSources.clear();

  for(size_t i = 0; i < sounds.size(); i++)
  {
    //alDeleteBuffers(1, &sounds.at(i)->id);
  }

  sounds.clear();

  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

std::string Environment::getAssetsDirectory()
{
  return instance->assetsDirectory;
}

Camera* Environment::getCamera()
{
  return instance->camera.get();
}

void Environment::repaint()
{
  display();
}

void Environment::display()
{
  instance->screenWidth = glutGet(GLUT_WINDOW_WIDTH);
  instance->screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
  glViewport(0, 0, instance->screenWidth, instance->screenHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(auto it = instance->renderTargets.begin();
    it != instance->renderTargets.end(); it++)
  {
    if((*it).lock())
    {
      (*it).lock()->internal->clear();
    }
    else
    {
      it = instance->renderTargets.erase(it);
    }
  }

  glPushMatrix();
  glLoadIdentity();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  getCamera()->applyProjection();

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    instance->entities.at(i)->render();
  }

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisableClientState(GL_NORMAL_ARRAY);

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    instance->entities.at(i)->postRender();
  }

  //glDisableClientState(GL_NORMAL_ARRAY);
  Gui::applyProjection();

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    instance->entities.at(i)->gui();
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glPopMatrix();

  glutSwapBuffers();
}

void Environment::idle()
{
  updateDeltaTime();

  for(size_t asi = 0; asi < instance->audioSources.size(); asi++)
  {
    ALuint& as = instance->audioSources.at(asi);
    ALint state = 0;

    alGetSourcei(as, AL_SOURCE_STATE, &state);

    if(state == AL_STOPPED)
    {
      //std::cout << "Deleting audio source" << std::endl;
      alDeleteSources(1, &as);
      instance->audioSources.erase(instance->audioSources.begin() + asi);
      asi--;
    }
  }

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    instance->entities.at(i)->tick();
  }

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    if(!instance->entities.at(i)->alive)
    {
      instance->entities.at(i)->_kill();
    }
  }

  for(size_t i = 0; i < instance->entities.size(); i++)
  {
    if(!instance->entities.at(i)->alive)
    {
      instance->entities.erase(instance->entities.begin() + i);
      i--;
    }
  }

  instance->downKeys.clear();
  Mouse::clearButtons();

  glutPostRedisplay();
}

void Environment::registerType(std::string type, void (*attachFunc)(Entity*))
{
  std::cout << "Registering " << type << std::endl;
  bool registered = false;

  for(size_t i = 0; i < 256; i++)
  {
    if(strcmp(registrations[i].type, "") == 0)
    {
      strcpy(registrations[i].type, type.c_str());
      registrations[i].func = attachFunc;
      registered = true;
      break;
    }
  }

  if(!registered)
  {
    std::cout << "Not enough registration slots" << std::endl;
    throw std::exception();
  }
}

void Environment::reshape(int width, int height)
{
  //instance->screenWidth = width;
  //instance->screenHeight = height;
  //glViewport(0, 0, width, height);
}

int Environment::getScreenWidth()
{
  return instance->screenWidth;
}

int Environment::getScreenHeight()
{
  return instance->screenHeight;
}

void Environment::setCamera(Camera* camera)
{
  instance->camera = camera;
}

void Environment::updateDeltaTime()
{
  float diffTime = 0;
  float deltaTime = 0;
  float targetFps = 60;

  if(instance->lastTime == 0)
  {
    instance->lastTime = (float)glutGet(GLUT_ELAPSED_TIME);
  }

  diffTime = (float)glutGet(GLUT_ELAPSED_TIME) - instance->lastTime;

  if(diffTime < 1000.0f / targetFps)
  {
    //std::cout << "Sleeping: " << ((1000.0f / targetFps) - diffTime) * 1000 << std::endl;
#ifdef _WIN32
    Sleep((int)((1000.0f / targetFps) - diffTime));
#else
    //usleep(((1000.0f / targetFps) - diffTime) * 900);
#endif
    diffTime = (float)glutGet(GLUT_ELAPSED_TIME) - instance->lastTime;
  }

  deltaTime = diffTime / 1000.0f;
  //std::cout << deltaTime * 1000.0f << std::endl;
  instance->lastTime = (float)glutGet(GLUT_ELAPSED_TIME);

  if(deltaTime > 0.25f)
  {
    deltaTime = 0.25f;
  }

  instance->deltaTime = deltaTime;
}

float Environment::getDeltaTime()
{
  return instance->deltaTime;
}

time_t Environment::getLifeTime()
{
  return time(NULL) - instance->startTime;
}

void Environment::exit()
{
  glutLeaveMainLoop();
}

void Environment::setTitle(std::string title)
{
#ifndef OPENGLD
  glutSetWindowTitle(title.c_str());
#endif
}

Entity* Environment::spawn(std::string name, Vector3 position, Vector3 rotation)
{
  bool found = false;
  Entity* ent = NULL;

  for(size_t r = 0; r < 256; r++)
  {
    RegisterAssociation ra = instance->registrations[r];

    if(strcmp("", ra.type) == 0) break;

    if(strcmp(name.c_str(), ra.type) == 0)
    {
      found = true;
      ent = Entity::create();
      ent->getTransform()->setPosition(position);
      ent->getTransform()->setRotation(rotation);
      ra.func(ent);
      break;
    }
  }

  if(found == false)
  {
    std::cout << "No type registered to " << name << std::endl;
    throw std::exception();
  }

  return ent;
}

}
