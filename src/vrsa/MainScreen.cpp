#include "MainScreen.h"
#include "Player.h"
#include "Piano.h"

void MainScreen::onInitialize()
{
  Environment::addEntity<Player>();

  //ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  //world = Model::load("models/WhiteKey/WhiteKey");
  //mr->setModel(world.get());

  Environment::addEntity<Piano>();
}
