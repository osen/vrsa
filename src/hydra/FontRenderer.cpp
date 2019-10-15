#include "hydra.h"

#include <GL/glew.h>

#include <iostream>

namespace hydra
{

void FontRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    std::cout << "No camera" << std::endl;
    return;
  }

  //float sizeMod = size.x;

  glPushMatrix();
  //Environment::getCamera()->applyProjection();
  Environment::getCamera()->applyView();

  Transform* t = getEntity()->getComponent<Transform>();
  t->applyModel();
  glRotatef(180, 0, 1, 0);

  //getEntity()->getTransform()->setRotation(origRotation);

  Vector2 size(font->getWidth(message), font->getHeight());
  //float sizeMod = 3.0f / size.x;
  float sizeMod = 1.0f;
  size *= sizeMod;
  glTranslatef(size.x / -2.0f, 0, 0);

  glFrontFace(GL_CW);

  if(message.length() > 0)
  {
    Glyph g = font->getGlyph(message.at(0));
    glBindTexture(GL_TEXTURE_2D, g.texture->internal->getId());

    for(size_t i = 0; i < message.length(); i++)
    {
      g = font->getGlyph(message.at(i));
      g.mesh->bind();

      size = Vector2(font->getWidth(std::string("") + message.at(i)), font->getHeight());
      size *= sizeMod;

      glPushMatrix();
      glScalef(size.x, size.y, 1);
      glDrawArrays(GL_TRIANGLES, 0, font->mesh->faces.size() * 3);
      glPopMatrix();

      glTranslatef(size.x + (1.0f * sizeMod), 0, 0);
    }
  }

  glFrontFace(GL_CCW);

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
