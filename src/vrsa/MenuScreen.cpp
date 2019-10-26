#include "MenuScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "Fade.h"
#include "VrButton.h"

#include <sstream>

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

  fr = Environment::addEntity<FontRenderer>();
  fr->setMessage("Hello");
  fr->setFont(Font::load("fonts/DroidWhiteLarge"));
  fr->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -45));
  fr->getEntity()->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));

  setInterval(2);
}

void MenuScreen::onTick()
{
  if(increaseButton->isClicked())
  {
    Environment::exit();
  }
}

void MenuScreen::setInterval(int interval)
{
  this->interval = interval;

  std::stringstream ss;
  ss << "Interval: " << interval;

  fr->setMessage(ss.str());
}

