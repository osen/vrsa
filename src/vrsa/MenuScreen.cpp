#include "MenuScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "Fade.h"
#include "VrButton.h"
#include "QuestionScreen.h"

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
  nextButton->setTexture(Texture::load("buttons/next"));
  nextButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -1, -3));
  nextButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  nextButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  quitButton = Environment::addEntity<VrButton>();
  quitButton->setTexture(Texture::load("buttons/exit"));
  quitButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2.5f, -1, -4));
  quitButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  quitButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  questionButton = Environment::addEntity<VrButton>();
  questionButton->setTexture(Texture::load("buttons/exam"));
  questionButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(2.5f, -1, -4));
  questionButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  questionButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  increaseButton = Environment::addEntity<VrButton>();
  increaseButton->setTexture(Texture::load("buttons/increase"));
  increaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(1, 0, -2));
  increaseButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  increaseButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  decreaseButton = Environment::addEntity<VrButton>();
  decreaseButton->setTexture(Texture::load("buttons/decrease"));
  decreaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-1, 0, -2));
  decreaseButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  decreaseButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

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
    setInterval(interval + 1);
  }

  if(decreaseButton->isClicked())
  {
    setInterval(interval - 1);
  }

  if(quitButton->isClicked())
  {
    Environment::exit();
  }

  if(nextButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MainScreen>();
  }

  if(questionButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<QuestionScreen>();
  }
}

void MenuScreen::setInterval(int interval)
{
  if(interval > 8 || interval < 1)
  {
    return;
  }

  this->interval = interval;

  std::stringstream ss;
  ss << "Interval: " << interval;

  fr->setMessage(ss.str());
}

