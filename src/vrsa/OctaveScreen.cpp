#include "OctaveScreen.h"
#include "Player.h"
#include "Octave.h"
#include "Fade.h"
#include "VrButton.h"
#include "MainScreen.h"
#include "IntervalSelect.h"

void OctaveScreen::onInitialize(const OctaveConstruction& oc)
{
  Environment::addEntity<Fade>(Vector3(0, 0, 0), true);
  Environment::addEntity<Player>();

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  octave = Environment::addEntity<Octave>(oc);
  octave->setReadOnly(true);

  backButton = Environment::addEntity<VrButton>();
  backButton->setTexture(Texture::load("buttons/back"));
  backButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  backButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-6.0f, -2.5, -2));
  backButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  backButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  repeatButton = Environment::addEntity<VrButton>();
  repeatButton->setTexture(Texture::load("buttons/repeat"));
  repeatButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  repeatButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-1.0f, -2.5, -5.0));
  repeatButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  repeatButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  intervalsButton = Environment::addEntity<VrButton>();
  intervalsButton->setTexture(Texture::load("buttons/intervals"));
  intervalsButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  intervalsButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(1.0f, -2.5, -5));
  intervalsButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  intervalsButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
}

void OctaveScreen::onTick()
{
  if(intervalSelect)
  {
    int selected = intervalSelect->getSelected();

    if(selected != -1)
    {
      intervalSelect->getEntity()->kill();
      octave->setBackground(false);
      playlist.clear();
      playlist.push_back(2);
      playlist.push_back(8);
      octave->setPlaylist(playlist);
    }

    return;
  }

  if(backButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MainScreen>();
  }

  if(repeatButton->isClicked())
  {
    octave->setBackground(false);
    playlist.clear();
    playlist.push_back(5);
    playlist.push_back(8);
    octave->setPlaylist(playlist);
  }

  if(intervalsButton->isClicked())
  {
    octave->setBackground(true);
    intervalSelect = Environment::addEntity<IntervalSelect>();
  }
}

