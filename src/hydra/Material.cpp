#include "hydra.h"

namespace hydra
{

struct MaterialVariable
{
  std::string name;
  std::sr1::zero_initialized<int> type;

  rend::mat4 mat4Val;
};

#define VAR_MAT4 1

void Material::apply()
{
  for(std::sr1::vector<std::sr1::shared_ptr<MaterialVariable> >::iterator it = variables.begin();
    it != variables.end(); it++)
  {
    if((*it)->type == VAR_MAT4)
    {
      shader->internal->setUniform((*it)->name, (*it)->mat4Val);
    }
    else
    {
      throw Exception("TODO: Should not get here");
    }
  }
}

void Material::setShader(const std::sr1::shared_ptr<Shader>& shader)
{
  this->shader = shader;
}

void Material::setVariable(const std::string& name, const mat4& value)
{
  std::sr1::shared_ptr<MaterialVariable> var = getVariable(name, VAR_MAT4);
  var->mat4Val = value;
}

std::sr1::shared_ptr<MaterialVariable> Material::getVariable(const std::string& name, int type)
{
  for(std::sr1::vector<std::sr1::shared_ptr<MaterialVariable> >::iterator it = variables.begin();
    it != variables.end(); it++)
  {
    if((*it)->name == name)
    {
      if((*it)->type != type)
      {
        throw Exception("Invalid variable type");
      }

      return *it;
    }
  }

  std::sr1::shared_ptr<MaterialVariable> rtn = std::sr1::make_shared<MaterialVariable>();
  rtn->name = name;
  rtn->type = type;
  variables.push_back(rtn);

  return rtn;
}

}
