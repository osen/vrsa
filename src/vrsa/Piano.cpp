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

    if(k)
    {
      selectKey(k);
    }

    if(octaveButton)
    {
      Vector3 hitLocal;
      Vector3 hitWorld;

      if(octaveButton->getComponent<ModelCollider>()->
        colliding(r, hitLocal, hitWorld) == true)
      {
        Environment::clear();
      }
    }
  }
}

void Piano::selectKey(std::sr1::observer_ptr<Key> key)
{
  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    keys.begin(); it != keys.end(); it++)
  {
    (*it)->setSelected(0);
  }

  int keyIdx = 0;

  std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator kit = keys.begin();
  for(; kit != keys.end(); kit++)
  {
    if(*kit == key)
    {
      break;
    }

    keyIdx++;
  }

  //int oct = 7;

  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    kit + 1; it != keys.end(); it++)
  {
    if(it == kit + 13)
    {
      break;
    }

    (*it)->setSelected(2);
    //if((*it)->getType() == 0)
    //{
    //  oct--;
    //}

    //if(oct <= 0)
    //{
    //  break;
    //}
  }

  key->setSelected(1);

  if(keyIdx + 13 <= keys.size())
  {
    updateOctaveButton(key);
  }
  else
  {
    updateOctaveButton(std::sr1::observer_ptr<Key>());
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

void Piano::updateOctaveButton(std::sr1::observer_ptr<Key> key)
{
  if(!key)
  {
    if(octaveButton)
    {
      octaveButton->kill();
    }

    return;
  }

  if(!octaveButton)
  {
    octaveButton = Entity::create();
    octaveButton->addComponent<ModelCollider>();

    octaveButton->addComponent<ModelRenderer>()->setModel(
      Model::load("models/OctaveButton/OctaveButton"));
  }

  Transform* t = octaveButton->getComponent<Transform>();
  Transform* keyT = key->getEntity()->getComponent<Transform>();
  Vector3 pos = keyT->getPosition();
  pos.y = 3;
  t->setPosition(pos);
  t->lookAt(Vector3(0, 0, 0));
  t->rotate(Vector3(0, 180, 0));
  t->setScale(Vector3(2, 2, 2));
}

