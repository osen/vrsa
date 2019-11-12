#include "MenuScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "Fade.h"
#include "VrButton.h"
#include "QuestionScreen.h"
#include "KeyHelper.h"

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
  nextButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -1, -4));
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

/*
  increaseButton = Environment::addEntity<VrButton>();
  increaseButton->setTexture(Texture::load("buttons/increase"));
  increaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(2, 0, -4));
  increaseButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  increaseButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  decreaseButton = Environment::addEntity<VrButton>();
  decreaseButton->setTexture(Texture::load("buttons/decrease"));
  decreaseButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2, 0, -4));
  decreaseButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  decreaseButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  frName = Environment::addEntity<FontRenderer>();
  frName->setMessage("Name");
  frName->setFont(Font::load("fonts/DroidWhiteLarge"));
  frName->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 5, -45));
  frName->getEntity()->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));

  frInterval = Environment::addEntity<FontRenderer>();
  frInterval->setMessage("Interval");
  frInterval->setFont(Font::load("fonts/DroidWhiteLarge"));
  frInterval->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -2.5, -45));
  frInterval->getEntity()->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));

  setInterval(6);
*/
}

void MenuScreen::onTick()
{
/*
  if(increaseButton->isClicked())
  {
    setInterval(interval + 1);
  }

  if(decreaseButton->isClicked())
  {
    setInterval(interval - 1);
  }
*/

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

/*
void MenuScreen::setInterval(int interval)
{
  if(interval > 12 || interval < 0)
  {
    return;
  }

  this->interval = interval;

  std::stringstream ss;
  ss << "Interval: " << interval;

  frInterval->setMessage(ss.str());
  frName->setMessage(KeyHelper::intervalToName(interval));
}
*/

