#include "CompleteScreen.h"
#include "MenuScreen.h"
#include "Player.h"
#include "Fade.h"
#include "VrButton.h"

void CompleteScreen::onInitialize(const std::string& id)
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

  Entity* fe = Entity::create();
  FontRenderer* fr = fe->addComponent<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhiteLarge"));
  fe->getComponent<Transform>()->setPosition(Vector3(0, 5, -25));
  fe->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));
  fr->setMessage("Thank you for participating!");

  fe = Entity::create();
  fr = fe->addComponent<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhite"));
  fe->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));
  fe->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));
  fr->setMessage("Session: " + id);

  doneButton = Environment::addEntity<VrButton>();
  doneButton->setTexture(Texture::load("buttons/done"));
  doneButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(0, -2, -5));
  doneButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  doneButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  doneButton->getEntity()->getTransform()->setScale(Vector3(2, 2, 2));
  doneButton->setLabel("Finish");
}

void CompleteScreen::onTick()
{
  if(doneButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MenuScreen>();
  }
}

