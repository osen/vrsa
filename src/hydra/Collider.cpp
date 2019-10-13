#include "hydra.h"

#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace hydra
{

bool ModelCollider::colliding(Ray& ray, Vector3 &hitLocal, Vector3 &hitWorld)
{
  ModelRenderer* mr = getEntity()->getComponent<ModelRenderer>();
  Model* m = mr->getModel();
  glm::vec3 bary;
  
  Vector3 pos = getEntity()->getComponent<Transform>()->getPosition();
  glm::mat4 model = glm::translate(glm::mat4(1), pos);

  Vector3 rot = getEntity()->getComponent<Transform>()->getRotation();
  Vector3 sca = getEntity()->getComponent<Transform>()->getScale();

  model = glm::scale(model, glm::vec3(sca.x, sca.y, sca.z));

  model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));

  model = glm::translate(model, mr->getOffset());

  bool hit = false;
  float bestHitDist = std::numeric_limits<float>::max();

  for(size_t pi = 0; pi < m->parts.size(); pi++)
  {
    Part* p = m->parts.at(pi).get();

    for(size_t mgi = 0; mgi < p->materialGroups.size(); mgi++)
    {
      MaterialGroup* mg = p->materialGroups.at(mgi).get();
      Mesh *mesh = p->materialGroups.at(mgi)->mesh.get();

      for(size_t fi = 0; fi < mesh->faces.size(); fi++)
      {
        Face& f = mesh->faces.at(fi);

        Vector3 a = model * glm::vec4(f.a.position, 1.0f);
        Vector3 b = model * glm::vec4(f.b.position, 1.0f);
        Vector3 c = model * glm::vec4(f.c.position, 1.0f);

        if(glm::intersectRayTriangle(ray.origin, ray.direction, a, b, c, bary))
        {
          //Converting barycentric to cartesian to get hit point
          //Weird but see https://github.com/g-truc/glm/issues/6
          float hitDist = bary.z;

          hit = true;

          if(hitDist < bestHitDist)
          {
            bestHitDist = hitDist;
          }
        }
      }
    }
  }

  if(hit)
  {
    hitWorld = ray.origin + ray.direction*bestHitDist;
    hitLocal = glm::inverse(model) * glm::vec4(hitWorld,1.0f);

    return true;
  }

  //glm::vec3 a(-10, -10, 0);
  //glm::vec3 b(0, 10, 0);
  //glm::vec3 c(10, -10, 0);

  //return glm::intersectRayTriangle(ray.origin, ray.direction, a, b, c, bary);

  return false;
}

void Collider::onBegin()
{
  //lastPosition = getEntity()->getTransform()->getPosition();
  lastPosition = glm::vec3(0, -999, 0);
}

void Collider::onTick()
{
  std::vector<Entity*> worlds;
  Entity::findByTag("world", worlds);

  bool solved = false;
  Vector3 resp = getEntity()->getTransform()->getPosition();

  // TODO: pass previous resp as new position + offset.

  for(size_t i = 0; i < worlds.size(); i++)
  {
    Entity* world = worlds.at(i);

    WorldCollider* wc = world->getComponent<WorldCollider>();

    if(!wc)
    {
      std::cout << "No world collider found" << std::endl;
      return;
    }

    resp = wc->getCollisionResponse(
      resp + offset, size, solved, lastPosition + offset);

    resp -= offset;
  }

  if(solved)
  {
    getEntity()->getTransform()->setPosition(resp);
    lastPosition = resp;
  }
  else
  {
    getEntity()->getTransform()->setPosition(lastPosition);
  }
}

void Collider::setSize(Vector3 size)
{
  this->size = size;
}

void Collider::setOffset(Vector3 offset)
{
  this->offset = offset;
}

}
