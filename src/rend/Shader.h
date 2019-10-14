#include "mathutil.h"

#include <GL/glew.h>

#include <sr1/zero_initialized>
#include <sr1/noncopyable>
#include <sr1/memory>

#include <string>

namespace rend
{

struct Context;

struct Shader : public std::sr1::noncopyable
{
  ~Shader();

  GLuint getId();
  void setSource(const std::string source);

  void setUniform(const std::string variable, mat4 value);

private:
  friend struct Context;

  std::sr1::shared_ptr<Context> context;
  std::sr1::zero_initialized<GLuint> id;

};

}
