#include "MainScreen.h"
#include "Player.h"
#include "Piano.h"
#include "ButtonBar.h"
#include "Fade.h"
#include "VrButton.h"
#include "HelpScreen.h"
#include "QuestionScreen.h"
#include "MenuScreen.h"

void MainScreen::onInitialize()
{
  Environment::addEntity<Fade>(Vector3(1, 1, 1), true);
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>(0);

  std::sr1::shared_ptr<Shader> s = Shader::load("shaders/default");

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world.get());
  mr->getMaterial()->setShader(s);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  quitButton = Environment::addEntity<VrButton>();
  quitButton->setTexture(Texture::load("buttons/exit"));
  quitButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  quitButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -7));
  quitButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  quitButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  backButton = Environment::addEntity<VrButton>();
  backButton->setTexture(Texture::load("buttons/back"));
  backButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  backButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -7));
  backButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -2.5, -8));
  backButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  backButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  helpButton = Environment::addEntity<VrButton>();
  helpButton->setTexture(Texture::load("buttons/help"));
  helpButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  helpButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(2.5, -2.5, -7));
  helpButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  helpButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

/*
  examButton = Environment::addEntity<VrButton>();
  examButton->setTexture(Texture::load("buttons/exam"));
  examButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  examButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(2.5, -2.5, -7));
  examButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  examButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
*/

  Environment::addEntity<Piano>();
}

void MainScreen::onTick()
{
  if(quitButton->isClicked())
  {
    Environment::exit();
  }

  if(backButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MenuScreen>();
  }

  if(helpButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<HelpScreen>();
  }

/*
  if(examButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<QuestionScreen>();
  }
*/
}
