#include "hydra.h"

#include <memory>

namespace hydra
{

Entity* Entity::create()
{
  std::shared_ptr<Entity> rtn = std::make_shared<Entity>();
  rtn->self = rtn.get();
  rtn->transform = rtn->addComponent<Transform>();
  rtn->alive = true;
  rtn->enabled = true;

  Environment::instance->entities.push_back(rtn);

  return rtn.get();
}

void Entity::setEnabled(bool enabled)
{
  this->enabled = enabled;
}

void Entity::tick()
{
  if(!alive) return;
  if(!enabled) return;

  for(size_t i = 0; i < components.size(); i++)
  {
    if(!components.at(i)->disabled)
    {
      if(!components.at(i)->began)
      {
        components.at(i)->onBegin();
        components.at(i)->began = true;
      }

      components.at(i)->onTick();
    }
  }

  for(std::vector<std::shared_ptr<Component> >::iterator it = components.begin();
    it != components.end();)
  {
    if(!(*it)->alive)
    {
      it = components.erase(it);
    }
    else
    {
      it++;
    }
  }
}

void Entity::preGui()
{
  if(!alive) return;
  if(!enabled) return;

  for(size_t i = 0; i < components.size(); i++)
  {
    if(!components.at(i)->disabled)
    {
      components.at(i)->onPreGui();
    }
  }
}

void Entity::gui()
{
  if(!alive) return;
  if(!enabled) return;

  for(size_t i = 0; i < components.size(); i++)
  {
    if(!components.at(i)->disabled)
    {
      components.at(i)->onGui();
    }
  }
}

void Entity::setImmutable(bool immutable)
{
  this->immutable = immutable;
}

void Entity::_kill()
{
  if(immutable)
  {
    return;
  }

  for(size_t i = 0; i < components.size(); i++)
  {
    components.at(i)->onKill();
  }
}

void Entity::render()
{
  if(!alive) return;
  if(!enabled) return;

  for(size_t i = 0; i < components.size(); i++)
  {
    if(!components.at(i)->disabled)
    {
      components.at(i)->onRender();
    }
  }
}

void Entity::postRender()
{
  if(!alive) return;
  if(!enabled) return;

  for(size_t i = 0; i < components.size(); i++)
  {
    if(!components.at(i)->disabled)
    {
      components.at(i)->onPostRender();
    }
  }
}

Transform* Entity::getTransform()
{
  return transform.get();
}

void Entity::addTag(std::string tag)
{
  tags.push_back(tag);
}

bool Entity::hasTag(std::string tag)
{
  for(size_t i = 0; i < tags.size(); i++)
  {
    if(tags.at(i) == tag || tag == "*")
    {
      return true;
    }
  }

  return false;
}

void Entity::kill()
{
  if(immutable)
  {
    return;
  }

  alive = false;

  for(size_t i = 0; i < components.size(); i++)
  {
    components.at(i)->onDoKill();
  }
}

Entity* Entity::findByTag(std::string tag)
{
  for(size_t i = 0; i < Environment::instance->entities.size(); i++)
  {
    Entity* curr = Environment::instance->entities.at(i).get();

    if(curr->hasTag(tag))
    {
      return curr;
    }
  }

  return NULL;
}

void Entity::findByTag(std::string tag, std::vector<Entity*>& output)
{
  for(size_t i = 0; i < Environment::instance->entities.size(); i++)
  {
    Entity* curr = Environment::instance->entities.at(i).get();

    if(curr->hasTag(tag))
    {
      output.push_back(curr);
    }
  }
}

}
