#include "hydra.h"

namespace hydra
{

std::sr1::shared_ptr<RenderTarget> RenderTarget::create()
{
  std::sr1::shared_ptr<RenderTarget> rtn = std::sr1::make_shared<RenderTarget>();

  rtn->internal = Environment::getContext()->createRenderTexture();

  return rtn;
}

}
