#include "OctaveScreen.h"
#include "Player.h"
#include "ButtonBar.h"
//#include "Key.h"
#include "Octave.h"
#include "Fade.h"

void OctaveScreen::onInitialize(const OctaveConstruction& oc)
{
  Environment::addEntity<Fade>(Vector3(0, 0, 0), true);
  Environment::addEntity<Player>();
  Environment::addEntity<ButtonBar>(1);

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  Environment::addEntity<Octave>(oc);
}

