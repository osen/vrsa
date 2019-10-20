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
    Gui::texture(Vector2(10, 10), leftCamera->getRenderTarget());

    Gui::texture(Vector2(10 + leftCamera->getRenderTarget()->getWidth() + 10, 10),
      rightCamera->getRenderTarget());
  }

  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft, mono]", font.get());
}

