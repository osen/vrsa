#include "IntervalSelect.h"
#include "VrButton.h"

void IntervalSelect::onInitialize()
{
  selected = -1;

  add(0, 1, 0, Texture::load("slices/darkgreen"));

  add(0.7f, 0.6f, -45, Texture::load("slices/purple"));
  add(1, 0, -90, Texture::load("slices/orange"));
  add(0.7f, -0.6f, -135, Texture::load("slices/turq"));

  add(0, -1, 180, Texture::load("slices/yellow"));

  add(-0.7f, 0.6f, 45, Texture::load("slices/red"));
  add(-1, 0, 90, Texture::load("slices/blue"));
  add(-0.7f, -0.6f, 135, Texture::load("slices/green"));
}

void IntervalSelect::onKill()
{
  for(std::sr1::vector<std::sr1::observer_ptr<VrButton> >::iterator it =
    buttons.begin(); it != buttons.end(); it++)
  {
    (*it)->getEntity()->kill();
  }
}

void IntervalSelect::add(float x, float y, float r, std::sr1::observer_ptr<Texture> base)
{
  std::sr1::observer_ptr<VrButton> b = Environment::addEntity<VrButton>();
  b->setModel(Model::load("models/IntervalSlice/IntervalSlice"));
  b->setBaseTexture(base);
  b->getEntity()->getTransform()->setPosition(vec3(x, y, -4));
  b->getEntity()->getTransform()->setScale(vec3(2, 2.5f, 1));
  b->getEntity()->getTransform()->setRotation(vec3(0, 0, r));

  buttons.push_back(b);
}

int IntervalSelect::getSelected()
{
  return selected;
}

void IntervalSelect::onTick()
{
  for(size_t i = 0; i < buttons.size(); i++)
  {
    if(buttons.at(i)->isClicked())
    {
      selected = i;
    }
  }
}
