#include "HelpScreen.h"
#include "Player.h"
#include "ButtonBar.h"
#include "Fade.h"

void HelpScreen::onInitialize()
{
  Environment::addEntity<Fade>(Vector3(0, 0, 0), true);
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>(2);

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoomBaked/MusicRoom_lightmap");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  //getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  Entity* fe = Entity::create();
  FontRenderer* fr = fe->addComponent<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhiteLarge"));
  fr->setMessage("Help Topics");
  fe->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));
  //fe->getComponent<Transform>()->setRotation(Vector3(0, 180, 0));
  fe->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));
}
