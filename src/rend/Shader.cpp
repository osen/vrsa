#include "Shader.h"
#include "Context.h"
#include "Exception.h"
#include "Buffer.h"

#include <sr1/vector>

namespace rend
{

struct VariableInfo
{
  std::string name;
  std::sr1::zero_initialized<GLint> loc;
  std::sr1::zero_initialized<int> type;
  std::sr1::zero_initialized<bool> attrib;

  mat4 mat4Val;
  std::sr1::shared_ptr<Buffer> bufferVal;
};

Shader::~Shader()
{
  glDeleteProgram(id);
  context->pollForError();
}

GLuint Shader::getId()
{
  return id;
}

void Shader::setUniform(const std::string& variable, mat4 value)
{
  std::sr1::shared_ptr<VariableInfo> vi = getVariableInfo(variable, GL_FLOAT_MAT4, false);
  //if(vi->mat4Val == value) return;

  glUseProgram(id); context->pollForError();
  glUniformMatrix4fv(vi->loc, 1, false, glm::value_ptr(value)); context->pollForError();
  glUseProgram(0); context->pollForError();

  vi->mat4Val = value;
}

void Shader::setAttribute(const std::string& variable, const std::sr1::shared_ptr<Buffer>& value)
{
  GLenum type = value->type;
  std::sr1::shared_ptr<VariableInfo> vi = getVariableInfo(variable, type, true);
  //if(vi->bufferVal == value) return;

  int size = 0;

  if(type == GL_FLOAT) size = 1;
  else if(type == GL_FLOAT_VEC2) size = 2;
  else if(type == GL_FLOAT_VEC3) size = 3;
  else throw Exception("Invalid buffer type");

  glBindBuffer(GL_ARRAY_BUFFER, value->getId()); context->pollForError();
  //glUseProgram(id); context->pollForError();
  glVertexAttribPointer(vi->loc, size, GL_FLOAT, GL_FALSE, 0, 0); context->pollForError();
  //glUseProgram(0); context->pollForError();
  glEnableVertexAttribArray(vi->loc); context->pollForError();
  glBindBuffer(GL_ARRAY_BUFFER, 0); context->pollForError();

  vi->bufferVal = value;
}

std::sr1::shared_ptr<VariableInfo> Shader::getVariableInfo(const std::string& name, GLenum type, bool attrib)
{
  for(std::sr1::vector<std::sr1::shared_ptr<VariableInfo> >::iterator it = cache.begin();
    it != cache.end(); it++)
  {
    if((*it)->name == name)
    {
      if ((*it)->type != type || (*it)->attrib != attrib)
      {
        throw Exception("Variable requested as wrong type");
      }

      return *it;
    }
  }

  std::sr1::shared_ptr<VariableInfo> rtn = std::sr1::make_shared<VariableInfo>();
  rtn->name = name;
  rtn->attrib = attrib;
  rtn->type = type;

  GLsizei unusedA = 0;
  GLint unusedB = 0;
  GLenum rtnType = 0;

  if(attrib == false)
  {
    rtn->loc = glGetUniformLocation(id, name.c_str());
    context->pollForError();

    if(rtn->loc == -1)
    {
      throw Exception("The specified variable was not found in the shader");
    }

    glGetActiveUniform(id, rtn->loc, 0, &unusedA, &unusedB, &rtnType, NULL);
    context->pollForError();

    if(rtnType != type)
    {
      throw Exception("The requested uniform had the wrong type");
    }
  }
  else
  {
    rtn->loc = glGetAttribLocation(id, name.c_str());
    context->pollForError();

    if(rtn->loc == -1)
    {
      throw Exception("The specified variable was not found in the shader");
    }

    glGetActiveAttrib(id, rtn->loc, 0, &unusedA, &unusedB, &rtnType, NULL);
    context->pollForError();

    if(rtnType != type)
    {
      throw Exception("The requested attribute had the wrong type");
    }
  }

  cache.push_back(rtn);

  return rtn;
}

void Shader::setSource(const std::string& source)
{
  GLuint vertId = 0;
  GLuint fragId = 0;
  int success = 0;
  const GLchar* src = NULL;

  cache.clear();

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
