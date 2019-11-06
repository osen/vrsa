#include "QuestionScreen.h"
#include "Player.h"
#include "Fade.h"
#include "Octave.h"
#include "VrButton.h"
#include "MenuScreen.h"

void QuestionScreen::onInitialize()
{
  Environment::addEntity<Fade>(Vector3(0, 0, 0), true);
  Environment::addEntity<Player>();

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  //getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  Entity* fe = Entity::create();
  FontRenderer* fr = fe->addComponent<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhiteLarge"));
  fr->setMessage("Question: 1 / 24");
  fe->getComponent<Transform>()->setPosition(Vector3(0, 10, -25));
  //fe->getComponent<Transform>()->setRotation(Vector3(0, 180, 0));
  fe->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));

  OctaveConstruction oc;
  oc.index = 48;
  octave = Environment::addEntity<Octave>(oc);
  octave->setReadOnly(true);

  backButton = Environment::addEntity<VrButton>();
  backButton->setTexture(Texture::load("buttons/back"));
  backButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  backButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -5));
  backButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  backButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
}

void QuestionScreen::onTick()
{
  if(backButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MenuScreen>();
  }

  if(Keyboard::getKeyDown('j'))
  {
    playSound(vec3(0, 0, 1));
  }

  if(Keyboard::getKeyDown('k'))
  {
    playSound(vec3(0, 0, -1));
  }

  if(Keyboard::getKeyDown('h'))
  {
    playSound(vec3(-1, 0, 0));
  }

  if(Keyboard::getKeyDown('l'))
  {
    playSound(vec3(1, 0, 0));
  }
}

void QuestionScreen::playSound(vec3 pos)
{
  Transform* t = Environment::getCamera()->getEntity()->getComponent<Transform>();
  Vector4 res = Environment::getCamera()->getView() * vec4(pos, 1.0f);
  pos = res;
  std::sr1::observer_ptr<Sound> sound = Sound::load("audio/octave4/C4");
  sound->play(pos);
}

