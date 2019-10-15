#include "Context.h"
#include "Texture.h"
#include "Exception.h"
#include "Shader.h"
#include "Buffer.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace rend
{

std::sr1::shared_ptr<Context> Context::initialize(int argc, char* argv[])
{
  std::sr1::shared_ptr<Context> rtn = std::sr1::make_shared<Context>();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Rend Renderer");

  if(glewInit() != GLEW_OK)
  {
    throw Exception("Failed to initialize glew");
  }

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  return rtn;
}

std::sr1::shared_ptr<Shader> Context::createShader()
{
  GLuint id = 0;
  id = glCreateProgram();

  if(id == 0)
  {
    throw Exception("Failed to create shader program");
  }

  pollForError();

  std::sr1::shared_ptr<Shader> rtn = std::sr1::make_shared<Shader>();
  rtn->context = self.lock();
  rtn->id = id;

  return rtn;
}

std::sr1::shared_ptr<Texture> Context::createTexture()
{
  GLuint id = 0;
  glGenTextures(1, &id);
  pollForError();

  std::sr1::shared_ptr<Texture> rtn = std::sr1::make_shared<Texture>();
  rtn->context = self.lock();
  rtn->id = id;

  return rtn;
}

std::sr1::shared_ptr<Buffer> Context::createBuffer()
{
  GLuint id = 0;
  glGenBuffers(1, &id);
  pollForError();

  std::sr1::shared_ptr<Buffer> rtn = std::sr1::make_shared<Buffer>();
  rtn->context = self.lock();
  rtn->id = id;

  return rtn;
}

void Context::pollForError()
{
  while(true)
  {
    GLenum err = glGetError();

    if(err == GL_NO_ERROR)
    {
      break;
    }

    //throw Exception((char*)gluErrorString(err));
    std::cout << "Warning: " << (char*)gluErrorString(err) << std::endl;
  }
}

}
