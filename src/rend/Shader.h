#include "mathutil.h"

#include <GL/glew.h>

#include <sr1/zero_initialized>
#include <sr1/noncopyable>
#include <sr1/memory>
#include <sr1/vector>

#include <string>

namespace rend
{

struct Context;
struct VariableInfo;
struct Buffer;

struct Shader : public std::sr1::noncopyable
{
  ~Shader();

  GLuint getId();
  void setSource(const std::string& source);

  void setUniform(const std::string& variable, mat4 value);
  void setAttribute(const std::string& variable, const std::sr1::shared_ptr<Buffer>& value);

private:
  friend struct Context;

  std::sr1::shared_ptr<Context> context;
  std::sr1::zero_initialized<GLuint> id;
  std::sr1::vector<std::sr1::shared_ptr<VariableInfo> > cache;

  std::sr1::shared_ptr<VariableInfo> getVariableInfo(const std::string& name, GLenum type, bool attrib);

};

}
