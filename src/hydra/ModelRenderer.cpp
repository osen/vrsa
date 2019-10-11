#include "hydra.h"

#include <iostream>

//#define BUFFER_OFFSET(bytes) ((char*)0 + (bytes))
//#define BUFFER_OFFSET(bytes) ((GLubyte*)NULL + (bytes))
//#define BUFFER_OFFSET(bytes) (GLvoid*)(sizeof(GLfloat) * bytes)
//#define BUFFER_OFFSET(bytes) (GLvoid*)(bytes)
#define BUFFER_OFFSET(bytes) ((GLvoid*)(sizeof(GLfloat) * bytes))

namespace hydra
{

void ModelRenderer::setOffset(Vector3 offset)
{
  this->offset = offset;
}

Vector3 ModelRenderer::getOffset()
{
  return offset;
}

void ModelRenderer::onTick()
{
  if(animations.size() < 1) return;

  frame += animations.at(0)->fps * Environment::getDeltaTime();

  if(frame >= animations.at(0)->frames.size())
  {
    frame = 0;
  }
}

void ModelRenderer::attachAnimation(Animation* animation)
{
  for(size_t i = 0; i < animations.size(); i++)
  {
    if(animations.at(i) == animation)
    {
      return;
    }
  }
  animations.push_back(animation);
}

void ModelRenderer::detachAnimation(Animation* animation)
{
  for(size_t i = 0; i < animations.size(); i++)
  {
    if(animations.at(i) == animation)
    {
      animations.erase(animations.begin() + i);
      break;
    }
  }
}

Vector2 ModelRenderer::getScreenCoordinate()
{
  return screenCoordinate;
}

int mrand_between(int min, int max)
{
  return std::rand() % (max + 1 - min) + min;
}

void ModelRenderer::updateScreenPosition(Transform* cameraTransform)
{
  Vector3 a = getEntity()->getTransform()->getPosition();
  Vector3 b = cameraTransform->getPosition();
  Vector3 r = cameraTransform->getRotation();

  float angle = -(atan2(b.z - a.z, b.x - a.x)) * (180.0f / 3.14f);
  angle += 270;

  float p = angle - r.y;

  while(p >= 360) p -= 360;
  while(p <= 0) p += 360;

  if(p < 90 || p > 270)
  {
    screenCoordinate = Vector2(-hydra::Environment::getScreenWidth(), -hydra::Environment::getScreenHeight());
    return;
  }

  GLdouble wx = 0;
  GLdouble wy = 0;
  GLdouble wz = 0;

  GLdouble model[16];
  GLdouble projection[16];
  GLint viewport[16];

  // TODO
/*
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  gluProject(0, 0, 0, model, projection, viewport, &wx, &wy, &wz);
  screenCoordinate = Vector2(wx, hydra::Environment::getScreenHeight() - wy);
*/
}

void ModelRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    //std::cout << "No camera" << std::endl;
    return;
  }

  // TODO
/*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
*/

  glPushMatrix();
  //Environment::getCamera()->applyProjection();
  Environment::getCamera()->applyView();

  Transform* t = getEntity()->getComponent<Transform>();
  t->applyModel();

  glTranslatef(
    offset.x,
    offset.y,
    offset.z);

  //Vector3 offset = model->getCenter();
  //glTranslatef(-offset.x, -offset.y, -offset.z);
  //updateScreenPosition(Environment::getCamera()->getEntity()->getComponent<Transform>());

  GLfloat light_ambient[] = { 0.5f, 0.5f, 0.4f, 1.0f };
  GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.6f, 1.0f };
  //GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat light_position[] = { 0.5f, 1.0f, 1.0f, 0.0f };

// TODO
/*
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  GLfloat light_diffuse2[] = { 0.3f, 0.3f, 0.2f, 1.0f };
  GLfloat light_position2[] = { -1.0f, -0.5f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
*/

  //static double amount = 0;

  int r = mrand_between(0, model->parts.size());

  glTranslatef(
    -model->getCenter().x,
    -model->getCenter().y,
    -model->getCenter().z);

  //glPushMatrix();

  for(size_t pi = 0; pi < model->parts.size(); pi++)
  {
    glPushMatrix();

    glTranslatef(
      model->parts.at(pi)->offset.x,
      model->parts.at(pi)->offset.y,
      model->parts.at(pi)->offset.z);

    //glRotatef(amount, 1, 0, 0);
    //amount +=0.1f;

    for(size_t ai = 0; ai < animations.size(); ai++)
    {
      animations.at(ai)->apply(model->parts.at(pi).get(), frame);
    }

    for(size_t mgi = 0; mgi < model->parts.at(pi)->materialGroups.size(); mgi++)
    {
      Mesh* mesh = model->parts.at(pi)->materialGroups.at(mgi)->mesh.get();

/*
      glBindBuffer(GL_ARRAY_BUFFER, mesh->positionBuffer);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->texCoordBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer);
      glNormalPointer(GL_FLOAT, 0, 0);
*/

      glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer);
      glVertexPointer(3, GL_FLOAT, 8 * sizeof(GLfloat), 0);
      glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GLfloat), BUFFER_OFFSET(3));
      glNormalPointer(GL_FLOAT, 8 * sizeof(GLfloat), BUFFER_OFFSET(5));

      glBindTexture(GL_TEXTURE_2D, model->parts.at(pi)->materialGroups.at(mgi)->texture->id);

      //if(model->parts.at(pi)->name != "LeftLowerLeg" && model->parts.at(pi)->name != "RightLowerLeg") continue;
      glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);

      if(r == (int)pi)
      {
        updateScreenPosition(Environment::getCamera()->getEntity()->getComponent<Transform>());
      }
    }

    glPopMatrix();
  }

  //glPopMatrix();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // TODO
/*
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
*/

  glPopMatrix();
}

void ModelRenderer::setModel(Model* model)
{
  this->model = model;
}

Model* ModelRenderer::getModel()
{
  return model.get();
}

}
