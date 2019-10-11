#include "hydra.h"

#include <iostream>

#define BUFFER_OFFSET(bytes) ((GLvoid*)(sizeof(GLfloat) * bytes))

namespace hydra
{

void FontRenderer::onPostRender()
{
  if(!Environment::getCamera())
  {
    std::cout << "No camera" << std::endl;
    return;
  }

  Vector2 size(font->getWidth(message), font->getHeight());
  float sizeMod = 3.0f / size.x;

  glPushMatrix();
  //Environment::getCamera()->applyProjection();
  Environment::getCamera()->applyView();

  Transform* t = getEntity()->getComponent<Transform>();

  hydra::Entity* player = hydra::Entity::findByTag("player");
  Vector3 origRotation = getEntity()->getTransform()->getRotation();

  if(player)
  {
    getEntity()->getTransform()->lookAt(player->getTransform()->getPosition());
  }

  t->rotate(Vector3(0, 180, 0));
  t->applyModel();
  getEntity()->getTransform()->setRotation(origRotation);

  size *= sizeMod;
  glTranslatef(size.x / -2.0f, 0, 0);

  if(message.length() > 0)
  {
    Glyph g = font->getGlyph(message.at(0));
    glBindTexture(GL_TEXTURE_2D, g.texture->id);

    for(size_t i = 0; i < message.length(); i++)
    {
      g = font->getGlyph(message.at(i));
/*
      glBindBuffer(GL_ARRAY_BUFFER, g.mesh->positionBuffer);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, g.mesh->texCoordBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
*/
      glBindBuffer(GL_ARRAY_BUFFER, g.mesh->buffer);
      glVertexPointer(3, GL_FLOAT, 8 * sizeof(GLfloat), 0);
      glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GLfloat), BUFFER_OFFSET(3));

      size = Vector2(font->getWidth(std::string("") + message.at(i)), font->getHeight());
      size *= sizeMod;

      glPushMatrix();
      glScalef(size.x, size.y, 1);
      glDrawArrays(GL_TRIANGLES, 0, font->mesh->faces.size() * 3);
      glPopMatrix();

      glTranslatef(size.x + (1.0f * sizeMod), 0, 0);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glPopMatrix();
}

void FontRenderer::setFont(Font *font)
{
  this->font = font;
}

void FontRenderer::setMessage(std::string message)
{
  this->message = message;
}

}
