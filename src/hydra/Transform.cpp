#include "hydra.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/vector_angle.hpp"

namespace hydra
{

void Transform::onInitialize()
{
  scale = Vector3(1, 1, 1);
}

void Transform::onKill()
{

}

void Transform::onTick()
{

}

void Transform::setScale(Vector3 scale)
{
  this->scale = scale;
}

Vector3 Transform::getScale()
{
  return scale;
}

void Transform::setPosition(Vector3 position)
{
  this->position = position;
  lastPosition = position;
}

void Transform::setRotation(Vector3 rotation)
{
  this->rotation = rotation;
}

void Transform::rotate(Vector3 rotation)
{
  setRotation(this->rotation + rotation);
}

Vector3 Transform::getPosition()
{
  return position;
}

Vector3 Transform::getRotation()
{
  return rotation;
}

/****************************************************************************
 * getRelative
 *
 * Project the position parameter respective to the direction of the entity.
 * For example, to obtain the position 10 units diagonal of an object in both
 * the X and Y axis, use:
 *
 *   diagonalPosition = getRelative(10, 0, 10);
 *
 * To make an object move left by 1 unit, use:
 *
 *   this->position = getRelative(-1, 0, 0);
 *
 * This will be useful for adding billboards and labels infront of objects.
 * The moveRelative method would have a similar concept.
 ****************************************************************************/
Vector3 Transform::getRelative(Vector3 position)
{
  Vector4 pos4 = Vector4(position, 1);

  Matrix mat = glm::translate(getPosition());

  mat = mat * glm::rotate(glm::radians(rotation.x), Vector3(1,0,0)) *
              glm::rotate(glm::radians(rotation.y), Vector3(0,1,0)) *
              glm::rotate(glm::radians(rotation.z), Vector3(0,0,1));

  return mat * pos4;
}

Vector3 Transform::getForward()
{
  Vector4 forward = Vector4(0,0,1,1);
 
  Matrix rot = glm::rotate(glm::radians(rotation.x), Vector3(1,0,0)) *
               glm::rotate(glm::radians(rotation.y), Vector3(0,1,0)) *
               glm::rotate(glm::radians(rotation.z), Vector3(0,0,1));

  return glm::normalize((Vector3)(rot * forward));
}

Vector3 Transform::getRight()
{
  //Assumes a global up vector of (0,1,0)
  return glm::cross(Vector3(0,1,0), getForward());
}

//Does not use collision checking
void Transform::moveTowards(Vector3 target, float speed)
{
  //Vector3 dir = glm::normalize(target - position);
  //position += dir * speed;
}

void Transform::moveForward(float speed)
{
  //position = getRelative(Vector3(0, 0, -1.0f * speed));
  //setRotation(rotation);
  //glm::mat4 rot = glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  //glm::vec4 rel = rot * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  //btVector3 res(rel.x, rel.y, rel.z);
  position = getRelative(Vector3(0, 0, -1) * speed);
}

void Transform::moveBackward(float speed)
{
  //position = getRelative(Vector3(0, 0, 1.0f * speed));
  //setRotation(rotation);
  //rigidBody->translate(btVector3(0, 0, 1.0f * speed));
  position = getRelative(Vector3(0, 0, 1) * speed);
}

void Transform::moveRight(float speed)
{
  //setRotation(rotation);
  //rigidBody->translate(btVector3(1.0f * speed, 0, 0));
  //position = getRelative(Vector3(1.0f * speed, 0, 0));
  position = getRelative(Vector3(1, 0, 0) * speed);
}

void Transform::moveLeft(float speed)
{
  //position = getRelative(Vector3(-1.0f * speed, 0, 0));
  //setRotation(rotation);
  //rigidBody->translate(btVector3(-1.0f * speed, 0, 0));
  position = getRelative(Vector3(-1, 0, 0) * speed);
}

void Transform::move(Vector3 direction, float speed)
{
  position = getRelative(direction * speed);
}

void Transform::lookAt(Vector3 target)
{
  Vector3 pos = getPosition();
/*
  Vector3 toTarget = -glm::normalize(target - pos);
  Vector3 forward = getForward();
  //Getting angle between two vectors then rotating by that with forward as axis
  Matrix rot = glm::rotate(glm::angle(forward, toTarget), forward);
  rotation = (Vector3)(Vector4(rotation, 1) * rot);
*/
  rotation.y = -(atan2(target.z - pos.z, target.x - pos.x)) * (180.0f / 3.14f);
  rotation.y += 270;
  setRotation(rotation);
}

void Transform::applyModel()
{
  Vector3 p = getPosition();
  glTranslatef(p.x, p.y, p.z);
  glRotatef(getRotation().y, 0, 1, 0);
  glRotatef(getRotation().x, 1, 0, 0);
  glScalef(scale.x, scale.y, scale.z);
}

mat4 Transform::getModel()
{
  mat4 rtn(1.0f);

  rtn = rend::translate(rtn, getPosition());
  rtn = rend::rotate(rtn, getRotation().y, vec3(0, 1, 0));
  rtn = rend::rotate(rtn, getRotation().x, vec3(1, 0, 0));
  rtn = rend::scale(rtn, scale);

  return rtn;
}

}
