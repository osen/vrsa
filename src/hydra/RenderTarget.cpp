#include "hydra.h"

namespace hydra
{

std::sr1::shared_ptr<RenderTarget> RenderTarget::create()
{
  std::sr1::shared_ptr<RenderTarget> rtn = std::sr1::make_shared<RenderTarget>();

  rtn->internal = Environment::getContext()->createRenderTexture();

  return rtn;
}

RenderTarget::~RenderTarget()
{
/*
  // TODO: Environment should possibly still be around at this point.
  if(!Environment::instance) return;

  for(auto it = Environment::instance->renderTargets.begin();
    it != Environment::instance->renderTargets.end(); it++)
  {
    if((*it).lock().get() == this)
    {
      it = Environment::instance->renderTargets.erase(it);
      break;
    }
  }
*/
}

std::sr1::shared_ptr<rend::TextureAdapter> RenderTarget::getInternal()
{
  return internal;
}

void RenderTarget::setSize(int width, int height)
{
  internal->setSize(width, height);
}

int RenderTarget::getWidth()
{
  return internal->getWidth();
}

int RenderTarget::getHeight()
{
  return internal->getHeight();
}

}
