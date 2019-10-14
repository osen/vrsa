#include "Shader.h"
#include "Context.h"
#include "Exception.h"

#include <sr1/vector>

namespace rend
{

Shader::~Shader()
{
  glDeleteProgram(id);
  context->pollForError();
}

GLuint Shader::getId()
{
  return id;
}

void Shader::setUniform(const std::string variable, mat4 value)
{

}

void Shader::setSource(const std::string source)
{
  GLuint vertId = 0;
  GLuint fragId = 0;
  int success = 0;
  const GLchar* src = NULL;

  std::string vertSrc = "";
  vertSrc += "#version 120\n";
  vertSrc += "#define VERTEX\n";
  vertSrc += source;
  src = vertSrc.c_str();

  vertId = glCreateShader(GL_VERTEX_SHADER);
  context->pollForError();

  glShaderSource(vertId, 1, &src, NULL);
  context->pollForError();

  glCompileShader(vertId);
  context->pollForError();

  glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
  context->pollForError();

  if(!success)
  {
    int length = 0;
    glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &length);
    context->pollForError();

    std::sr1::vector<char> infoLog(length);
    glGetShaderInfoLog(vertId, length, NULL, &infoLog.at(0));
    context->pollForError();

    glDeleteShader(vertId);
    context->pollForError();

    std::string msg = &infoLog.at(0);
    throw Exception(msg);
  }

  std::string fragSrc = "";
  fragSrc += "#version 120\n";
  fragSrc += "#define FRAGMENT\n";
  fragSrc += source;
  src = fragSrc.c_str();

  fragId = glCreateShader(GL_FRAGMENT_SHADER);
  context->pollForError();

  glShaderSource(fragId, 1, &src, NULL);
  context->pollForError();

  glCompileShader(fragId);
  context->pollForError();

  glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
  context->pollForError();

  if(!success)
  {
    int length = 0;
    glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &length);
    context->pollForError();

    std::sr1::vector<char> infoLog(length);
    glGetShaderInfoLog(fragId, length, NULL, &infoLog.at(0));
    context->pollForError();

    glDeleteShader(fragId);
    context->pollForError();

    std::string msg = &infoLog.at(0);
    throw Exception(msg);
  }

  glAttachShader(id, vertId);
  context->pollForError();

  glAttachShader(id, fragId);
  context->pollForError();

  glLinkProgram(id);
  context->pollForError();

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  context->pollForError();

  if(!success)
  {
    int length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    context->pollForError();

    std::sr1::vector<char> infoLog(length);
    glGetProgramInfoLog(id, length, NULL, &infoLog.at(0));
    context->pollForError();

    std::string msg = &infoLog.at(0);
    throw Exception(msg);
  }

  glDetachShader(id, vertId);
  context->pollForError();

  glDetachShader(id, fragId);
  context->pollForError();

  glDeleteShader(vertId);
  context->pollForError();

  glDeleteShader(fragId);
  context->pollForError();
}

}
