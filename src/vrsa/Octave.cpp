#include "Octave.h"
#include "Key.h"
#include "KeyHelper.h"

#include <glm/ext.hpp>

#include <sstream>

void Octave::onInitialize(const OctaveConstruction& oc)
{
  this->startIndex = oc.index;

  for(int i = 0; i < 13; i++)
  {
    KeyInfo ki = KeyHelper::octaveLayout(oc.index, i);

    float f = -90.0f + (180.0f * ki.position);

    glm::mat4 m(1.0f);
    m = glm::rotate(m, glm::radians(f), glm::vec3(0, 1, 0));
    m = glm::translate(m, glm::vec3(0, 0, 8));
    glm::vec3 pos(m * glm::vec4(0, 0, 0, 1));

    if(ki.type == 1)
    {
      pos.y += 1.05f;
    }

    Key* key = Environment::addEntity<Key>(i);
    key->setType(ki.type);
    key->setSound(Sound::load(ki.audioPath));
    Transform* t = key->getEntity()->getTransform();
    pos.z *= -1;
    t->setPosition(pos);
    t->setRotation(Vector3(0, -f, 0));
    t->setScale(Vector3(3.5f, 1.5f, 1));
    keys.push_back(key);
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

