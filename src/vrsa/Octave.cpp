#include "Octave.h"
#include "Key.h"

#include <glm/ext.hpp>

#include <sstream>

void Octave::onInitialize(int index)
{
  //std::cout << "Octave Index: " << index << std::endl;
  this->index = index;
  loadSounds();

  int type = 0;
  size_t soundIdx = 0;

  float ang = 180.0f / (float)sounds.size();
  float f = -90;
  int posIdx = 0;

  for(int i = 0; i < sounds.size(); i++)
  {
    float f = -90.0f + (ang * (float)posIdx);
    glm::mat4 m(1.0f);
    m = glm::rotate(m, glm::radians(f), glm::vec3(0, 1, 0));
    m = glm::translate(m, glm::vec3(0, 0, 8));
    glm::vec3 pos(m * glm::vec4(0, 0, 0, 1));
    int keyType = 0;

    if(type == 1 ||
      type == 3 ||
      type == 7 ||
      type == 9 ||
      type == 11)
    {
      pos.y += 1.05f;
      keyType = 1;
    }

    if(type != 5 && type != 13)
    {
      Key* key = Environment::addEntity<Key>();
      key->setType(keyType);
      key->setSound(sounds.at(soundIdx));
      soundIdx++;
      Transform* t = key->getEntity()->getTransform();
      pos.z *= -1;
      t->setPosition(pos);
      t->setRotation(Vector3(0, -f, 0));
      t->setScale(Vector3(4, 1.5f, 1));
      keys.push_back(key);
    }
    else
    {
      // Not actually a key so do not consume a sound here.
      // Position idx maintained separately.
      i--;
    }

    type++;
    if(type > 13) type = 0;
    posIdx ++;
  }
}

void Octave::onTick()
{
  if(Mouse::getButtonDown(0) == true)
  {
    Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());
    Ray r = Environment::getCamera()->createRay(mpos);

    std::sr1::observer_ptr<Key> k = getKey(r);

    if(k)
    {
      selectKey(k);
    }
  }
}

std::sr1::observer_ptr<Key> Octave::getKey(Ray ray)
{
  float dist = std::numeric_limits<float>::max();
  std::sr1::observer_ptr<Key> rtn;

  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    keys.begin(); it != keys.end(); it++)
  {
    ModelCollider* mc = (*it)->getEntity()->getComponent<ModelCollider>();

    Vector3 hitLocal;
    Vector3 hitWorld;

    if(mc->colliding(ray, hitLocal, hitWorld) == true)
    {
      float d = glm::distance(hitWorld, ray.origin);

      if(d < dist)
      {
        dist = d;
        rtn = *it;
      }
    }
  }

  return rtn;
}

void Octave::selectKey(std::sr1::observer_ptr<Key> key)
{
  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    keys.begin(); it != keys.end(); it++)
  {
    (*it)->setSelected(0);
  }

  key->setSelected(1);
  key->play();
}

void Octave::loadSounds()
{
  std::stringstream trunk;
  trunk << "audio/octave";
  trunk << index;
  trunk << "/";

  std::stringstream base;
  base << index;

  sounds.push_back(Sound::load(trunk.str() + "C" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "CS" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "D" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "DS" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "E" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "F" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "FS" + base.str()));
  sounds.push_back(Sound::load(trunk.str() + "G" + base.str()));

  if(index != 8)
  {
    sounds.push_back(Sound::load(trunk.str() + "GS" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "A" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "AS" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "B" + base.str()));
  }
}

