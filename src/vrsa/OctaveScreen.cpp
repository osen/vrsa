#include "OctaveScreen.h"
#include "Player.h"
#include "ButtonBar.h"

void OctaveScreen::onInitialize()
{
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>();

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));
}

