#include "hydra.h"

#include <iostream>

namespace hydra
{

void WorldRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    //std::cout << "No camera" << std::endl;
    return;
  }

  glPushMatrix();
  //Environment::getCamera()->applyProjection();
  Environment::getCamera()->applyView();
  getEntity()->getTransform()->applyModel();

  for(size_t i = 0; i < world->materialGroups.size(); i++)
  {
    if(!world->materialGroups.at(i)->texture)
    {
      continue;
    }

    Mesh* mesh = world->materialGroups.at(i)->mesh.get();

    mesh->bind();
    glBindTexture(GL_TEXTURE_2D, world->materialGroups.at(i)->texture->internal->getId());

    glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_EQUAL, 1);
    glAlphaFunc(GL_GEQUAL, 0.8f);

    glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);

    glDisable(GL_ALPHA_TEST);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glPopMatrix();
}

void WorldRenderer::onInitialize()
{
  world = getEntity()->getComponent<World>();
}

}
