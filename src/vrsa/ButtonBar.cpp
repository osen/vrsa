#include "ButtonBar.h"

void ButtonBar::onInitialize()
{
  Entity* e = Entity::create();
  ModelRenderer* mr = e->addComponent<ModelRenderer>();
  mr->setModel(Model::load("models/ExitButton/ExitButton"));
  octaveCollider = e->addComponent<ModelCollider>();
  e->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -5));
  e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
}

void ButtonBar::onTick()
{
  if(Mouse::getButtonDown(0) == true)
  {
    Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());
    Ray r = Environment::getCamera()->createRay(mpos);

    Vector3 hitLocal;
    Vector3 hitWorld;

    if(octaveCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::exit();
    }
  }
}
