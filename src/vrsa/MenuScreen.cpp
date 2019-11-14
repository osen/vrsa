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

  Entity* bg = Entity::create();
  bg->getTransform()->setScale(vec3(95, 35, 40));
  bg->getTransform()->setPosition(Vector3(0, 0, -90));
  mr = bg->addComponent<ModelRenderer>();
  mr->setModel(Model::load("models/Background/Background"));

  nextButton = Environment::addEntity<VrButton>();
  nextButton->setTexture(Texture::load("buttons/training"));
  nextButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -2, -5));
  nextButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  nextButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  nextButton->getEntity()->getTransform()->setScale(Vector3(2, 2, 2));
  nextButton->setLabel("Training");

  quitButton = Environment::addEntity<VrButton>();
  quitButton->setTexture(Texture::load("buttons/exit"));
  quitButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-3.5f, -2, -4));
  quitButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  quitButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  quitButton->getEntity()->getTransform()->setScale(Vector3(2, 2, 2));
  quitButton->setLabel("Exit");

  questionButton = Environment::addEntity<VrButton>();
  questionButton->setTexture(Texture::load("buttons/exam"));
  questionButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(3.5f, -2, -4));
  questionButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  questionButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  questionButton->getEntity()->getTransform()->setScale(Vector3(2, 2, 2));
  questionButton->setLabel("Questions");
}

void MenuScreen::onTick()
{
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

