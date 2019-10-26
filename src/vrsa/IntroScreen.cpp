#include "IntroScreen.h"
#include "MainScreen.h"
#include "MenuScreen.h"

void IntroScreen::onInitialize()
{
  logo = Texture::load("images/BULogo");
  //Environment::getCamera()->setClearColor(Vector4(1, 1, 1, 1));
  timeout = 1;
}

void IntroScreen::onTick()
{
  timeout -= Environment::getDeltaTime();

  if(timeout <= 0)
  {
    //Environment::addEntity<MainScreen>();
    Environment::addEntity<MenuScreen>();
    getEntity()->kill();
  }
}

void IntroScreen::onGui()
{
  Vector2 imgSize = Vector2(logo->getWidth(), logo->getHeight());

  Vector2 scrSize = Vector2(Environment::getScreenWidth(),
    Environment::getScreenHeight());

  Gui::texture(Vector2(scrSize.x / 2 - imgSize.x / 2,
    scrSize.y / 2 - imgSize.y / 2), logo);
}

