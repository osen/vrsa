#include "IntervalSelect.h"
#include "VrButton.h"

void IntervalSelect::onInitialize()
{
  selected = -1;

  add(0, 1, 0, Texture::load("slices/darkgreen"), "1");

  add(0.7f, 0.6f, -45, Texture::load("slices/purple"), "2");
  add(1, 0, -90, Texture::load("slices/orange"), "3");
  add(0.7f, -0.6f, -135, Texture::load("slices/turq"), "4");

  add(0, -1, 180, Texture::load("slices/yellow"), "5");

  add(-0.7f, -0.6f, 135, Texture::load("slices/green"), "6");
  add(-1, 0, 90, Texture::load("slices/blue"), "7");
  add(-0.7f, 0.6f, 45, Texture::load("slices/red"), "8");
}

void IntervalSelect::onKill()
{
  for(std::sr1::vector<std::sr1::observer_ptr<VrButton> >::iterator it =
    buttons.begin(); it != buttons.end(); it++)
  {
    (*it)->getEntity()->kill();
  }

  for(std::sr1::vector<std::sr1::observer_ptr<FontRenderer> >::iterator it =
    labels.begin(); it != labels.end(); it++)
  {
    (*it)->getEntity()->kill();
  }
}

void IntervalSelect::add(float x, float y, float r, std::sr1::observer_ptr<Texture> base, const std::string& label)
{
  std::sr1::observer_ptr<VrButton> b = Environment::addEntity<VrButton>();
  b->setModel(Model::load("models/IntervalSlice/IntervalSlice"));
  b->setBaseTexture(base);
  b->getEntity()->getTransform()->setPosition(vec3(x, y, -4));
  b->getEntity()->getTransform()->setScale(vec3(2, 2.5f, 1));
  b->getEntity()->getTransform()->setRotation(vec3(0, 0, r));
  b->setTexture(Texture::load("buttons/slice"));

  buttons.push_back(b);

  FontRenderer* fr = Environment::addEntity<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhite"));
  fr->setMessage(label);
  fr->setColor(vec3(0, 0, 0));
  fr->getEntity()->getTransform()->setPosition(vec3(x, y - 0.2f, -3.9));
  fr->getEntity()->getTransform()->setScale(vec3(0.01f, 0.01f, 1.0f));

  labels.push_back(fr);
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
