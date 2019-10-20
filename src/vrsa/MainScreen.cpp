#include "MainScreen.h"
#include "Player.h"
#include "Piano.h"
#include "ButtonBar.h"
#include "Fade.h"

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

  Environment::addEntity<Piano>();
}
