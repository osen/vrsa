#include "Key.h"

void Key::onInitialize(int index)
{
  this->index = index;
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  material = mr->getMaterial();
  material->setShader(Shader::load("shaders/key"));
  setType(0);
  ModelCollider* mc = getEntity()->addComponent<ModelCollider>();
}

void Key::onTick()
{
  if(selected == 1)
  {
    time += Environment::getDeltaTime();
  }
  else
  {
    time = 0;
  }

  material->setVariable("u_Time", time);
}

void Key::setSound(std::sr1::observer_ptr<Sound> sound)
{
  this->sound = sound;
}

void Key::play()
{
  sound->play();
}

void Key::setSelected(int selected)
{
  this->selected = selected;
  refresh();
}

void Key::setType(int type)
{
  this->type = type;
  refresh();
}

int Key::getType()
{
  return type;
}

void Key::refresh()
{
  if(type == 0 && selected == 0)
  {
    material->setVariable("u_Color", Vector4(1, 1, 1, 1));
  }
  else if(type == 0 && selected == 1)
  {
    material->setVariable("u_Color", Vector4(0, 0, 1, 1));
  }
  else if(type == 0 && selected == 2)
  {
    material->setVariable("u_Color", Vector4(0.5, 0.5, 1, 1));
  }
  else if(type == 1 && selected == 0)
  {
    material->setVariable("u_Color", Vector4(0, 0, 0, 1));
  }
  else if(type == 1 && selected == 1)
  {
    material->setVariable("u_Color", Vector4(0, 0, 1, 1));
  }
  else if(type == 1 && selected == 2)
  {
    material->setVariable("u_Color", Vector4(0, 0, 0.5, 1));
  }

  if(type == 0)
  {
    getEntity()->getComponent<ModelRenderer>()->setModel(
    Model::load("models/WhiteKey/WhiteKey"));
  }
  else
  {
    getEntity()->getComponent<ModelRenderer>()->setModel(
    Model::load("models/BlackKey/BlackKey"));
  }
}

std::sr1::observer_ptr<Sound> Key::getSound()
{
  return sound;
}

int Key::getIndex()
{
  return index;
}

