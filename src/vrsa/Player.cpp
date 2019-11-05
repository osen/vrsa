#include "Player.h"
#include "VrManager.h"
#include "Pointer.h"

#include <iostream>

void Player::onInitialize()
{
  leftCamera = getEntity()->addComponent<Camera>();
  leftCamera->setClearColor(Vector4(1.0f, 0.5f, 0.5f, 1));

  rightCamera = getEntity()->addComponent<Camera>();
  rightCamera->setClearColor(Vector4(0.5f, 0.5f, 1.0f, 1));

  std::sr1::observer_ptr<Entity> vrme = Entity::findByTag("vrmanager");

  if(!vrme)
  {
    vrme = Environment::addEntity<VrManager>()->getEntity();
  }

  std::sr1::observer_ptr<VrManager> vrm = vrme->getComponent<VrManager>();
  vrm->setCameras(leftCamera, rightCamera);

  pointer = Environment::addEntity<Pointer>(getEntity());
}

void Player::onTick()
{
  Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());

  if(Mouse::getButton(2) == true)
  {
    Vector2 mdelta = mpos - lastMouse;
    Vector3 rot = getEntity()->getComponent<Transform>()->getRotation();
    rot.y -= mdelta.x * 0.5f;
    rot.x -= mdelta.y * 0.5f;
    getEntity()->getComponent<Transform>()->setRotation(rot);
  }

  if(Mouse::getButton(1) == true)
  {
    getEntity()->getComponent<Transform>()->move(Vector3(0, 0, -1), 25 * Environment::getDeltaTime());
  }

  lastMouse = mpos;
}

