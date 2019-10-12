#include "Player.h"

void Player::onInitialize()
{
  camera = getEntity()->addComponent<Camera>();
  camera->setClearColor(Vector4(0.5f, 0.5f, 0.5f, 1));

  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, 0));
  //getEntity()->getComponent<Transform>()->setRotation(Vector3(0, 180, 0));
}
