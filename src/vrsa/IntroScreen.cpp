#include "IntroScreen.h"
#include "MainScreen.h"
#include "VrManager.h"

void IntroScreen::onInitialize()
{
  logo = Texture::load("images/BULogo");
  Environment::getCamera()->setClearColor(Vector4(1, 1, 1, 1));
  timeout = 1;
}

void IntroScreen::onTick()
{
  timeout -= Environment::getDeltaTime();

  if(timeout <= 0)
  {
    Environment::addEntity<MainScreen>();
    Environment::addEntity<VrManager>();
    getEntity()->kill();
  }
}

void IntroScreen::onGui()
{
  Gui::texture(Vector2(100, 100), logo.get());
}

