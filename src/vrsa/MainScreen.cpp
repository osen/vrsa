#include "MainScreen.h"
#include "Player.h"
#include "Piano.h"
#include "ButtonBar.h"

void MainScreen::onInitialize()
{
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>(0);

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world.get());
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  Environment::addEntity<Piano>();
}
