#include "MenuScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "Fade.h"
#include "VrButton.h"

void MenuScreen::onInitialize()
{
  Environment::addEntity<Fade>(Vector3(1, 1, 1), true);
  Environment::addEntity<Player>();

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* room = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(room);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  nextButton = Environment::addEntity<VrButton>();
  nextButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(1, 0, -2));

  increaseButton = Environment::addEntity<VrButton>();
  increaseButton->setTexture(Texture::load("buttons/increase"));
  increaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(1, 0, -2));

  decreaseButton = Environment::addEntity<VrButton>();
  decreaseButton->setTexture(Texture::load("buttons/decrease"));
  decreaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-1, 0, -2));
}

