#include "hydra.h"

namespace hydra
{

Component::Component()
{
  alive = true;
}

Component::~Component() {}
void Component::onInitialize() {}
void Component::onBegin() {}
void Component::onTick() {}
void Component::onRender() {}
void Component::onPostRender() {}
void Component::onPreGui() {}
void Component::onGui() {}
void Component::onKill() {}
void Component::onDoKill() {}

void Component::setEnabled(bool enabled)
{
  disabled = !enabled;
}

bool Component::getEnabled()
{
  return !disabled;
}

Entity* Component::getEntity()
{
  return entity.get();
}

void Component::kill()
{
  alive = false;
}

}
