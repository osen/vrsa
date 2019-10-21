#include "hydra.h"

namespace hydra
{

struct MaterialVariable
{
  std::string name;
  std::sr1::zero_initialized<int> type;

  std::sr1::zero_initialized<float> floatVal;
  rend::vec4 vec4Val;
  rend::mat4 mat4Val;
  std::sr1::observer_ptr<TextureAdapter> textureVal;
};

#define VAR_MAT4 1
#define VAR_VEC4 2
#define VAR_TEXTURE 3
#define VAR_FLOAT 4

void Material::apply()
{
  for(std::sr1::vector<std::sr1::shared_ptr<MaterialVariable> >::iterator it = variables.begin();
    it != variables.end(); it++)
  {
    if((*it)->type == VAR_MAT4)
    {
      shader->internal->setUniform((*it)->name, (*it)->mat4Val);
    }
    else if((*it)->type == VAR_VEC4)
    {
      shader->internal->setUniform((*it)->name, (*it)->vec4Val);
    }
    else if((*it)->type == VAR_TEXTURE)
    {
      shader->internal->setSampler((*it)->name, (*it)->textureVal->getInternal());
    }
    else if((*it)->type == VAR_FLOAT)
    {
      shader->internal->setUniform((*it)->name, (*it)->floatVal);
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

void Material::setVariable(const std::string& name, float value)
{
  std::sr1::shared_ptr<MaterialVariable> var = getVariable(name, VAR_FLOAT);
  var->floatVal = value;
}

void Material::setVariable(const std::string& name, const mat4& value)
{
  std::sr1::shared_ptr<MaterialVariable> var = getVariable(name, VAR_MAT4);
  var->mat4Val = value;
}

void Material::setVariable(const std::string& name, const vec4& value)
{
  std::sr1::shared_ptr<MaterialVariable> var = getVariable(name, VAR_VEC4);
  var->vec4Val = value;
}

void Material::setVariable(const std::string& name, const std::sr1::observer_ptr<TextureAdapter>& value)
{
  std::sr1::shared_ptr<MaterialVariable> var = getVariable(name, VAR_TEXTURE);
  var->textureVal = value;
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
