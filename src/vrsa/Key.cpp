#include "Key.h"

void Key::onInitialize()
{
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  setType(0);
  ModelCollider* mc = getEntity()->addComponent<ModelCollider>();
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
  Model* model = NULL;

  if(type == 0 && selected == 0)
  {
    model = Model::load("models/WhiteKey/WhiteKey");
  }
  else if(type == 0 && selected == 1)
  {
    model = Model::load("models/WhiteKey/selected/WhiteKey");
  }
  else if(type == 0 && selected == 2)
  {
    model = Model::load("models/WhiteKey/highlight/WhiteKey");
  }
  else if(type == 1 && selected == 0)
  {
    model = Model::load("models/BlackKey/BlackKey");
  }
  else if(type == 1 && selected == 1)
  {
    model = Model::load("models/BlackKey/selected/BlackKey");
  }
  else if(type == 1 && selected == 2)
  {
    model = Model::load("models/BlackKey/highlight/BlackKey");
  }

  getEntity()->getComponent<ModelRenderer>()->setModel(model);
}
