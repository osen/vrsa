#include "hydra.h"

#include <SDL2/SDL.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
#endif

#include <iostream>

#include <string.h>
#include <time.h>

namespace hydra
{

float rotation = 0;
float direction = 0.5f;

std::shared_ptr<Environment> Environment::instance;
RegisterAssociation Environment::registrations[256];

Environment::~Environment() { }

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
#if _WIN32
  assetsDirectory = "../share/vrsa/";
#else
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
#endif

  std::cout << "Assets Path: " << assetsDirectory << std::endl;
}

std::sr1::shared_ptr<rend::Context> Environment::getContext()
{
  return instance->graphics;
}

void Environment::initializePre(int argc, char *argv[])
{
  instance = std::make_shared<Environment>();

  instance->setupPaths(argv[0]);

  instance->screenWidth = 640;
  instance->screenHeight = 480;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw Exception("Failed to initialize window system");
  }

  instance->window = SDL_CreateWindow("VRSA",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    instance->screenWidth, instance->screenHeight,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if(!instance->window)
  {
    throw Exception("Failed to create window");
  }

  instance->glContext = SDL_GL_CreateContext(instance->window);

  if(!instance->glContext)
  {
    throw Exception("Failed to create OpenGL context");
  }

  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    // Failed to enable Vsync
  }

  instance->graphics = rend::Context::initialize();
  instance->openAudio();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Gui::initialize();
}

void Environment::initializePost()
{
  //spawn(entryComponent, Vector3(), Vector3());

  instance->startTime = time(NULL);

  SDL_SetWindowFullscreen(instance->window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  instance->running = true;
  SDL_Event e = {0};

  while(instance->running)
  {
    while(SDL_PollEvent(&e) != 0)
    {
      if(e.type == SDL_QUIT)
      {
        instance->running = false;
      }
      else if(e.type == SDL_KEYDOWN)
      {
        Keyboard::keyboard(e.key.keysym.sym, 0, 0);
      }
      else if(e.type == SDL_KEYUP)
      {
        Keyboard::keyboardUp(e.key.keysym.sym, 0, 0);
      }
      else if(e.type == SDL_MOUSEMOTION)
      {
        int mx = 0;
        int my = 0;
        SDL_GetMouseState(&mx, &my);
        Mouse::motion(mx, my);
      }
      else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
      {
        int mx = 0;
        int my = 0;
        SDL_GetMouseState(&mx, &my);
        Mouse::mouse(e.button.button, e.button.state, mx, my);
      }
    }

    idle();
    display();
    SDL_GL_SwapWindow(instance->window);
  }

  instance->closeAudio();

  // TODO: Make this more elegant
  SDL_GLContext glContext = instance->glContext;
  SDL_Window* window = instance->window;
  instance.reset();
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
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
  int width = 0;
  int height = 0;

  SDL_GetWindowSize(instance->window, &width, &height);
  instance->screenWidth = width;
  instance->screenHeight = height;
  glViewport(0, 0, instance->screenWidth, instance->screenHeight);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(auto it = instance->cameras.begin();
    it != instance->cameras.end();)
  {
    if((*it))
    {
      it++;
    }
    else
    {
      it = instance->cameras.erase(it);
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glPushMatrix();

  for(auto it = instance->cameras.begin();
    it != instance->cameras.end(); it++)
  {
    instance->camera = *it;

    if(instance->camera->getRenderTarget())
    {
      Vector4 cc = instance->camera->getClearColor();
      glClearColor(cc.x, cc.y, cc.z, cc.w);
      instance->camera->getRenderTarget()->internal->clear();
    }

    glLoadIdentity();
    getCamera()->applyProjection();

    for(size_t i = 0; i < instance->entities.size(); i++)
    {
      instance->entities.at(i)->render();
    }
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
    instance->lastTime = (float)SDL_GetTicks();
  }

  diffTime = (float)SDL_GetTicks() - instance->lastTime;

  if(diffTime < 1000.0f / targetFps)
  {
    //std::cout << "Sleeping: " << ((1000.0f / targetFps) - diffTime) * 1000 << std::endl;
#ifdef _WIN32
    Sleep((int)((1000.0f / targetFps) - diffTime));
#else
    //usleep(((1000.0f / targetFps) - diffTime) * 900);
#endif
    diffTime = (float)SDL_GetTicks() - instance->lastTime;
  }

  deltaTime = diffTime / 1000.0f;
  //std::cout << deltaTime * 1000.0f << std::endl;
  instance->lastTime = (float)SDL_GetTicks();

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
  instance->running = false;
}

void Environment::setTitle(std::string title)
{
  SDL_SetWindowTitle(instance->window, title.c_str());
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
