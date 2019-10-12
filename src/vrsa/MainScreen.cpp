#include "MainScreen.h"
#include "Player.h"
#include "Piano.h"
#include "ButtonBar.h"

void MainScreen::onInitialize()
{
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>();

  //ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  //world = Model::load("models/WhiteKey/WhiteKey");
  //mr->setModel(world.get());

  Environment::addEntity<Piano>();
}
