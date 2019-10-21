#include "VrManager.h"

void VrManager::onInitialize(
  std::sr1::observer_ptr<Camera> leftCamera,
  std::sr1::observer_ptr<Camera> rightCamera)
{
  font = Font::load("fonts/DroidWhite");

  this->leftCamera = leftCamera;
  this->rightCamera = rightCamera;
}

void VrManager::onTick()
{
  if(Keyboard::getKeyDown('v'))
  {
    if(leftCamera->getRenderTarget())
    {
      leftCamera->setRenderTarget(std::sr1::shared_ptr<RenderTarget>());
      rightCamera->setRenderTarget(std::sr1::shared_ptr<RenderTarget>());
    }
    else
    {
      leftCamera->setRenderTarget(RenderTarget::create());
      rightCamera->setRenderTarget(RenderTarget::create());
    }
  }
}

void VrManager::onGui()
{
  if(leftCamera->getRenderTarget())
  {
    Vector2 size(Environment::getScreenWidth(),
      Environment::getScreenHeight());

    size.x -= 30;
    size.x /= 2;

    size.y -= 20;

    Gui::texture(Vector4(10, 10, size.x, size.y), leftCamera->getRenderTarget());

    Gui::texture(Vector4(10 + size.x + 10, 10, size.x, size.y),
      rightCamera->getRenderTarget());
  }

  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft, mono]", font.get());
}

