#include "Pointer.h"

void Pointer::onInitialize(std::sr1::observer_ptr<Entity> parent)
{
  getEntity()->addTag("pointer");
  this->parent = parent;
  mr = getEntity()->addComponent<ModelRenderer>();
  mr->setDepthTest(false);
  mr->setModel(Model::load("models/Pointer/Pointer"));
  getEntity()->getTransform()->setScale(vec3(2.5f, 2.5f, 1));

  distance = 9;
}

void Pointer::onTick()
{
  Transform* t = parent->getTransform();

/*
  mat4 m = t->getModel();
  m = translate(m, vec3(0, 0, -10));
  vec4 res = m * vec4(0, 0, 0, 1);
  getEntity()->getTransform()->setPosition(vec3(res.x, res.y, res.z));
*/

  vec2 p = Mouse::getMotion();
  p *= 0.1f;
  position += p;

  if(position.y < -30)
  {
    position.y = -30;
  }
  if(position.y > 30)
  {
    position.y = 30;
  }

  mat4 m(1.0f);
  m = translate(m, t->getPosition());
  m = rotate(m, rend::radians(-position.x), vec3(0, 1, 0));
  m = rotate(m, rend::radians(-position.y), vec3(1, 0, 0));
  m = translate(m, vec3(0, 0, -distance));
  vec4 res = m * vec4(0, 0, 0, 1);
  getEntity()->getTransform()->setPosition(vec3(res.x, res.y, res.z));
  getEntity()->getTransform()->lookAt(t->getPosition());
  getEntity()->getTransform()->rotate(vec3(0, 180, 0));
}

void Pointer::restAgainst(vec3 position)
{
  distance = rend::distance(parent->getTransform()->getPosition(), position);
  collision = true;
  //mr->setDepthTest(false);
}

void Pointer::onGui()
{
  // A bit hacky but reset the distance
  if(!collision)
  {
    distance = 10;
    //mr->setDepthTest(true);
  }

  collision = false;
}

Ray Pointer::getRay()
{
  Ray r;

  Transform* t = parent->getTransform();
  r.origin = t->getPosition();

  r.direction = getEntity()->getTransform()->getPosition() - r.origin;
  r.direction = rend::normalize(r.direction);

  return r;
}

