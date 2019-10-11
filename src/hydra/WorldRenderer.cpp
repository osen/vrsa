#include "hydra.h"

#include <iostream>

#define BUFFER_OFFSET(bytes) ((GLvoid*)(sizeof(GLfloat) * bytes))

namespace hydra
{

void WorldRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    //std::cout << "No camera" << std::endl;
    return;
  }

/*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
*/

  glPushMatrix();
  //Environment::getCamera()->applyProjection();
  Environment::getCamera()->applyView();
  getEntity()->getTransform()->applyModel();

/*
  GLfloat light_ambient[] = { 0.5, 0.5, 0.4, 1.0 };
  GLfloat light_diffuse[] = { 0.7, 0.7, 0.6, 1.0 };
  //GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 0.5f, 1.0f, 1.0f, 0.0f };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  GLfloat light_diffuse2[] = { 0.3, 0.3, 0.2, 1.0 };
  GLfloat light_position2[] = { -1.0f, -0.5f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
*/

  for(size_t i = 0; i < world->materialGroups.size(); i++)
  {
    if(!world->materialGroups.at(i)->texture)
    {
      continue;
    }

    Mesh* mesh = world->materialGroups.at(i)->mesh.get();

/*
    glBindBuffer(GL_ARRAY_BUFFER, mesh->positionBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->texCoordBuffer);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    //For some reason drawing without the normals causes an
    //access violation on my system at home on glDrawArrays
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer);
    glNormalPointer(GL_FLOAT, 0, 0);
*/

    glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer);
    glVertexPointer(3, GL_FLOAT, 8 * sizeof(GLfloat), 0);
    glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GLfloat), BUFFER_OFFSET(3));
    glNormalPointer(GL_FLOAT, 8 * sizeof(GLfloat), BUFFER_OFFSET(5));
    
    GLuint newId = 0;
    glBindTexture(GL_TEXTURE_2D, newId);
    world->materialGroups.at(i)->texture->id = newId;
    glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_EQUAL, 1);
    glAlphaFunc(GL_GEQUAL, 0.8f);

#ifdef USE_GLUTEN
    gnDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);
#else
    glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);
#endif

    glDisable(GL_ALPHA_TEST);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
/*
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
*/

  glPopMatrix();
}

void WorldRenderer::onInitialize()
{
  world = getEntity()->getComponent<World>();
}

}
