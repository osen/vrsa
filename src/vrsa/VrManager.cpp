#include "VrManager.h"

void VrManager::onInitialize()
{
  font = Font::load("fonts/DroidWhite");

  Camera* camera = Environment::getCamera();
  //camera->setRenderTarget(RenderTarget::create());
}

void VrManager::onTick()
{
  if(Keyboard::getKeyDown('v'))
  {
    Camera* camera = Environment::getCamera();

    if(camera->getRenderTarget())
    {
      camera->setRenderTarget(std::sr1::shared_ptr<RenderTarget>());
      //camera->setRenderTarget(RenderTarget::create());
    }
    else
    {
      camera->setRenderTarget(RenderTarget::create());
    }
  }
}

void VrManager::onGui()
{
  Camera* camera = Environment::getCamera();

  if(camera->getRenderTarget())
  {
    Gui::texture(Vector2(10, 10), camera->getRenderTarget());
    Gui::texture(Vector2(10 + camera->getRenderTarget()->getWidth() + 10, 10), camera->getRenderTarget());
  }

  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft, mono]", font.get());


}

