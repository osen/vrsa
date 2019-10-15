#include "hydra.h"

#include <GL/glu.h>

#include <glm/gtc/matrix_transform.hpp>

namespace hydra
{

Vector4 Camera::clearColor(0, 0, 0.2f, 1);

void Camera::setClearColor(Vector4 clearColor)
{
  Camera::clearColor = clearColor;
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}

Vector4 Camera::getClearColor()
{
  return clearColor;
}

void Camera::onInitialize()
{
  Environment::setCamera(this); // TODO: No this
}

void Camera::setOffset(Vector3 offset)
{
  this->offset = offset;
}

void Camera::onRender()
{
  // If the camera is quite far away, we want to teleport rather than zoom
  if(glm::distance(getEntity()->getTransform()->getPosition(),
    smoothPosition) > 3.5f)
  {
    smoothPosition = getEntity()->getTransform()->getPosition();
  }
  else
  {
    float spy = smoothPosition.y * 3.0f;
    spy += getEntity()->getTransform()->getPosition().y;
    spy /= 4.0f;
    smoothPosition *= 6.0f;
    smoothPosition += getEntity()->getTransform()->getPosition();
    smoothPosition /= 7.0f;
    smoothPosition.y = spy;
  }
}

void Camera::applyProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(0, 1024, 1024, 0, 0.0f, 1.0f);

  gluPerspective(65.0f, (float)Environment::getScreenWidth() / (float)Environment::getScreenHeight(),
    0.1f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
}

mat4 Camera::getProjection()
{
  mat4 rtn = rend::perspective(rend::radians(65.0f),
    (float)Environment::getScreenWidth() / (float)Environment::getScreenHeight(),
    0.1f, 1000.0f);

  return rtn;
}

mat4 Camera::getView()
{
  mat4 rtn = getEntity()->getTransform()->getModel();
  rtn = rend::inverse(rtn);

  rtn = rend::translate(rtn, -(smoothPosition + offset));

  return rtn;
}

void Camera::applyView()
{
  //glTranslatef(250, 250, 0);
  //glRotatef(90, 0, 0, 1);
  //glRotatef(25.0f, 1, 0, 0);
  //std::cout << getEntity().get() << std::endl;
  glRotatef(getEntity()->getTransform()->getRotation().x, -1, 0, 0);
  glRotatef(getEntity()->getTransform()->getRotation().y, 0, -1, 0);
  //glScalef(2, 2, 2);
  //smoothPosition = getEntity()->getTransform()->getPosition();
  Vector3 p = smoothPosition;
  p += offset;
  glTranslatef(-p.x, -p.y, -p.z);
}

Ray Camera::createRay(Vector2 screenPosition)
{
  float sw = Environment::getScreenWidth();
  float sh = Environment::getScreenHeight();

  glm::mat4 projection = glm::perspective(glm::radians(65.0f), sw / sh, 0.1f, 1000.0f);

  glm::mat4 view;
  view = glm::rotate(view, glm::radians(getEntity()->getTransform()->getRotation().x), glm::vec3(-1, 0, 0));
  view = glm::rotate(view, glm::radians(getEntity()->getTransform()->getRotation().y), glm::vec3(0, -1, 0));

  Vector3 p = smoothPosition;
  //Vector3 p = getEntity()->getTransform()->getPosition();
  p += offset;
  view = glm::translate(view, p);
  //view = glm::translate(view, glm::vec3(100, 100, 100));

  glm::vec3 worldSpaceNear = glm::unProject(glm::vec3(screenPosition.x, sh - screenPosition.y, 0.0),
    view, projection, glm::vec4(0.0, 0.0, sw, sh));

  glm::vec3 worldSpaceFar = glm::unProject(glm::vec3(screenPosition.x, sh - screenPosition.y, 1.0),
    view, projection, glm::vec4(0.0, 0.0, sw, sh));

  Ray rtn;

  rtn.origin = smoothPosition + offset;
  rtn.direction = glm::normalize(worldSpaceFar - worldSpaceNear);
  //rtn.direction = worldSpaceFar - worldSpaceNear;

  return rtn;
}

}
