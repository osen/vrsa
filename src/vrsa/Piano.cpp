#include "Piano.h"
#include "Key.h"

#include <glm/ext.hpp>

#include <iostream>

void Piano::onInitialize()
{
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
    }

    type++;

    if(type > 13) type = 0;
  }
}
