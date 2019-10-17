#include "Piano.h"
#include "Key.h"
#include "OctaveScreen.h"
#include "Octave.h"
#include "KeyHelper.h"

#include <glm/ext.hpp>

#include <iostream>
#include <sstream>

void Piano::onInitialize()
{
  getEntity()->addTag("piano");
  //preloadKeys();
  //loadSounds();

  for(int i = 0; i < 104; i++)
  {
    KeyInfo ki = KeyHelper::pianoLayout(i);

    float f = -90.0f + (180.0f * ki.position);

    glm::mat4 m(1.0f);
    m = glm::rotate(m, glm::radians(f), glm::vec3(0, 1, 0));
    m = glm::translate(m, glm::vec3(0, 0, 21));
    glm::vec3 pos(m * glm::vec4(0, 0, 0, 1));

    if(ki.type == 1)
    {
      pos.y += 0.75f;
    }

    Key* key = Environment::addEntity<Key>(i);
    key->setType(ki.type);
    key->setSound(Sound::load(ki.audioPath));
    Transform* t = key->getEntity()->getTransform();
    pos.z *= -1;
    t->setPosition(pos);
    t->setRotation(Vector3(0, -f, 0));
    keys.push_back(key);
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
      //selectKeyOctave(k);
    }

    if(octaveButton)
    {
      Vector3 hitLocal;
      Vector3 hitWorld;

      if(octaveButton->getComponent<ModelCollider>()->
        colliding(r, hitLocal, hitWorld) == true)
      {
        OctaveConstruction oc;
        oc.index = octaveIndex;

        Environment::clear();
        Environment::addEntity<OctaveScreen>(oc);
      }
    }
  }
}

void Piano::selectKeyOctave(std::sr1::observer_ptr<Key> key)
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

  octaveIndex = (keyIdx / 12);
  size_t octaveStartIdx = octaveIndex * 12;

  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    keys.begin() + octaveStartIdx; it != keys.end() && it != keys.begin() + octaveStartIdx + 12; it++)
  {
    (*it)->setSelected(2);
  }

  key->setSelected(1);
  key->play();
  updateOctaveButton(*(keys.begin() + octaveStartIdx + 5));
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

  octaveIndex = keyIdx;

  for(std::sr1::vector<std::sr1::observer_ptr<Key> >::iterator it =
    kit + 1; it != keys.end(); it++)
  {
    if(it == kit + 13)
    {
      break;
    }

    (*it)->setSelected(2);
  }

  key->setSelected(1);
  key->play();

  if(keyIdx + 13 <= keys.size())
  {
    updateOctaveButton(keys.at(keyIdx + 6));
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

/*
void Piano::preloadKeys()
{
  Model::load("models/WhiteKey/WhiteKey");
  Model::load("models/WhiteKey/highlight/WhiteKey");
  Model::load("models/WhiteKey/selected/WhiteKey");
  Model::load("models/BlackKey/BlackKey");
  Model::load("models/BlackKey/highlight/BlackKey");
  Model::load("models/BlackKey/selected/BlackKey");
}

void Piano::loadSounds()
{
  for(size_t i = 0; i < 9; i++)
  {
    std::stringstream trunk;
    trunk << "audio/octave";
    trunk << i;
    trunk << "/";

    std::stringstream base;
    base << i;

    sounds.push_back(Sound::load(trunk.str() + "C" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "CS" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "D" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "DS" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "E" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "F" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "FS" + base.str()));
    sounds.push_back(Sound::load(trunk.str() + "G" + base.str()));

    if(i < 8)
    {
      sounds.push_back(Sound::load(trunk.str() + "GS" + base.str()));
      sounds.push_back(Sound::load(trunk.str() + "A" + base.str()));
      sounds.push_back(Sound::load(trunk.str() + "AS" + base.str()));
      sounds.push_back(Sound::load(trunk.str() + "B" + base.str()));
    }
  }
}
*/
