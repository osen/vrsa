#include "IntervalSelect.h"
#include "VrButton.h"
#include "KeyHelper.h"

void IntervalSelect::onInitialize()
{
  selected = -1;

  add(0, 1, 0, Texture::load("slices/darkgreen"), KeyHelper::intervalToSName(1));

  add(0.7f, 0.6f, -45, Texture::load("slices/purple"), " " + KeyHelper::intervalToSName(2));
  add(1, 0, -90, Texture::load("slices/orange"), KeyHelper::intervalToSName(3));
  add(0.7f, -0.6f, -135, Texture::load("slices/turq"), " " + KeyHelper::intervalToSName(4));

  add(0, -1, 180, Texture::load("slices/yellow"), KeyHelper::intervalToSName(5));

  add(-0.7f, -0.6f, 135, Texture::load("slices/green"), KeyHelper::intervalToSName(6));
  add(-1, 0, 90, Texture::load("slices/blue"), KeyHelper::intervalToSName(7));
  add(-0.7f, 0.6f, 45, Texture::load("slices/red"), KeyHelper::intervalToSName(8));

  label = Environment::addEntity<FontRenderer>();
  label->setFont(Font::load("fonts/DroidWhite"));
  label->setMessage("Select an interval");
  label->setColor(vec3(1, 1, 1));
  label->getEntity()->getTransform()->setPosition(vec3(0, 1.0f, -3.0));
  label->getEntity()->getTransform()->setScale(vec3(0.01f, 0.01f, 1.0f));
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

  label->getEntity()->kill();
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
  fr->setColor(vec3(0.1f, 0.1f, 0.1f));
  fr->getEntity()->getTransform()->setPosition(vec3(x, y - 0.1f, -3.9));
  fr->getEntity()->getTransform()->setScale(vec3(0.005f, 0.005f, 1.0f));

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

    if(buttons.at(i)->isHover())
    {
      label->setMessage(KeyHelper::intervalToName(i + 1));
    }
  }
}
