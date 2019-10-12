#include "Key.h"

void Key::onInitialize()
{
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  setType(0);
  ModelCollider* mc = getEntity()->addComponent<ModelCollider>();
}

void Key::setType(int type)
{
  this->type = type;

  Model* model = NULL;

  if(type == 0)
  {
    model = Model::load("models/WhiteKey/WhiteKey");
  }
  else
  {
    model = Model::load("models/BlackKey/BlackKey");
  }

  getEntity()->getComponent<ModelRenderer>()->setModel(model);
}
