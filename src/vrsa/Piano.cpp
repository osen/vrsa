#include "Piano.h"
#include "Key.h"

#include <glm/ext.hpp>

#include <iostream>

void Piano::onInitialize()
{
  getEntity()->addTag("piano");
  int type = 0;

  for(int i = -90; i <= 90; i += 2)
  {
    glm::mat4 m(1.0f);
    m = glm::rotate(m, glm::radians((float)i), glm::vec3(0, 1, 0));
    m = glm::translate(m, glm::vec3(0, 0, 16));
    glm::vec3 pos(m * glm::vec4(0, 0, 0, 1));
    int keyType = 0;

    if(type == 1 ||
      type == 3 ||
      type == 7 ||
      type == 9 ||
      type == 11)
    {
      pos.y += 0.75f;
      keyType = 1;
    }

    if(type != 5 && type != 13)
    {
      Key* key = Environment::addEntity<Key>();
      key->setType(keyType);
      Transform* t = key->getEntity()->getTransform();
      pos.z *= -1;
      t->setPosition(pos);
      t->setRotation(Vector3(0, -i, 0));
      keys.push_back(key);
    }

    type++;

    if(type > 13) type = 0;
  }
}

void Piano::onTick()
{
  if(Mouse::getButtonDown(0) == true)
  {
    Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());
    Ray r = Environment::getCamera()->createRay(mpos);

    std::sr1::observer_ptr<Key> k = getKey(r);

    std::cout << k.get() << " " << k << std::endl;
  }
}

std::sr1::observer_ptr<Key> Piano::getKey(Ray ray)
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
