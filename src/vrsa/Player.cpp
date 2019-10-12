#include "Player.h"
#include "Piano.h"
#include "Key.h"

#include <iostream>

void Player::onInitialize()
{
  camera = getEntity()->addComponent<Camera>();
  camera->setClearColor(Vector4(0.5f, 0.5f, 0.5f, 1));
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

  lastMouse = mpos;
}
