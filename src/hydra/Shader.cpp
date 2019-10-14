#include "hydra.h"

#include <fstream>

namespace hydra
{

std::sr1::shared_ptr<Shader> Shader::load(const std::string& path)
{
  for(size_t i = 0; i < Environment::instance->shaders.size(); i++)
  {
    if(Environment::instance->shaders.at(i)->path == path)
    {
      return Environment::instance->shaders.at(i);
    }
  }

  std::sr1::shared_ptr<Shader> rtn = std::sr1::make_shared<Shader>();
  rtn->path = path;
  Environment::instance->shaders.push_back(rtn);

  std::string p = Environment::getAssetsDirectory() + "/" + path + ".glsl";
  std::ifstream f(p.c_str());

  if(!f.is_open())
  {
    throw Exception("Failed to open shader");
  }

  std::string src;

  while(!f.eof())
  {
    std::string line;
    std::getline(f, line);
    src += line + "\n";
  }

  rtn->internal = Environment::getContext()->createShader();
  rtn->internal->setSource(src);

  return rtn;
}

std::string Shader::getPath()
{
  return path;
}

}
