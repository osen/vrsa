#include "IntervalSelect.h"
#include "VrButton.h"

void IntervalSelect::onInitialize()
{
  selected = -1;

  add(0, 1, 0);

  add(-0.7f, 0.6f, 45);
  add(-1, 0, 90);
  add(-0.7f, -0.6f, 135);

  add(0, -1, 180);

  add(0.7f, 0.6f, -45);
  add(1, 0, -90);
  add(0.7f, -0.6f, -135);
}

void IntervalSelect::onKill()
{
  for(std::sr1::vector<std::sr1::observer_ptr<VrButton> >::iterator it =
    buttons.begin(); it != buttons.end(); it++)
  {
    (*it)->getEntity()->kill();
  }
}

void IntervalSelect::add(float x, float y, float r)
{
  std::sr1::observer_ptr<VrButton> b = Environment::addEntity<VrButton>();
  b->setModel(Model::load("models/IntervalSlice/IntervalSlice"));
  b->getEntity()->getTransform()->setPosition(vec3(x, y, -5));
  b->getEntity()->getTransform()->setScale(vec3(2, 2, 1));
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
