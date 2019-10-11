#include "hydra.h"

namespace hydra
{

Texture* Gui::buttonTexture = NULL;

void Gui::initialize()
{
  buttonTexture = Texture::load("internal/textures/button");

  Environment::instance->guiMesh = std::make_shared<Mesh>();
  Vertex a;
  a.position = Vector3(0, 0, 0);
  a.texCoord = Vector3(0, 0, 0);
  Vertex b;
  b.position = Vector3(0, 1, 0);
  b.texCoord = Vector3(0, 1, 0);
  Vertex c;
  c.position = Vector3(1, 1, 0);
  c.texCoord = Vector3(1, 1, 0);
  Environment::instance->guiMesh->addFace(a, b, c);
  Vertex d;
  d.position = Vector3(1, 0, 0);
  d.texCoord = Vector3(1, 0, 0);
  Environment::instance->guiMesh->addFace(c, d, a);

  Environment::instance->guiMesh->generateVbos();
}

void Gui::applyProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, Environment::getScreenWidth(), Environment::getScreenHeight(),
    0, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void Gui::texture(Vector2 position, Texture* texture)
{
  Gui::texture(Vector4(position.x, position.y,
    texture->getWidth(), texture->getHeight()), texture);
}

void Gui::texture(Vector4 position, Texture* texture)
{
  applyProjection();

  Mesh* mesh = Environment::instance->guiMesh.get();
  mesh->bind();
  //glBindBuffer(GL_ARRAY_BUFFER, mesh->positionBuffer);
  //glVertexPointer(3, GL_FLOAT, 0, 0);
  //glBindBuffer(GL_ARRAY_BUFFER, mesh->texCoordBuffer);
  //glTexCoordPointer(2, GL_FLOAT, 0, 0);
  //glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer);
  //glNormalPointer(GL_FLOAT, 0, 0);

  glBindTexture(GL_TEXTURE_2D, texture->id);

  glPushMatrix();
  glTranslatef(position.x, position.y, 0);
  glScalef(position.z, position.w, 1);
  //glScalef(128, 128, 1);
  mesh->bind();
  glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);
  glPopMatrix();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

bool Gui::button(Vector4 position, std::string label)
{
  applyProjection();

  Mesh* mesh = Environment::instance->guiMesh.get();
  mesh->bind();
  glBindTexture(GL_TEXTURE_2D, buttonTexture->id);

  glPushMatrix();
  glTranslatef(position.x, position.y, 0);
  //glScalef(position.z, position.w, 1);
  glScalef(128, 128, 1);
  glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);
  glPopMatrix();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  return false;
}

void Gui::text(Vector2 position, std::string label, Font* font)
{
  if(!font)
  {
    font = Font::load("fonts/army");
  }

  Vector2 size(font->getWidth(label), font->getHeight());

  glPushMatrix();
  applyProjection();

  glTranslatef(position.x, position.y + size.y, 0);

  if(label.length() > 0)
  {
  Glyph g = font->getGlyph(label.at(0));
  glBindTexture(GL_TEXTURE_2D, g.texture->id);

  for(size_t i = 0; i < label.length(); i++)
  {
    g = font->getGlyph(label.at(i));
    g.mesh->bind();

/*
    glBindBuffer(GL_ARRAY_BUFFER, g.mesh->positionBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, g.mesh->texCoordBuffer);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
*/

    size = Vector2(font->getWidth(std::string("") + label.at(i)), font->getHeight());

    glPushMatrix();
    glScalef(size.x, -size.y, 1);
    glDrawArrays(GL_TRIANGLES, 0, g.mesh->faces.size() * 3);
    glPopMatrix();

    glTranslatef(size.x + 1.0f, 0, 0);
  }
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glPopMatrix();
}

}
