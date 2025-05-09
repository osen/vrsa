#include "hydra.h"

namespace hydra
{

Texture* Gui::buttonTexture = NULL;

// TODO: Place as member
std::sr1::shared_ptr<rend::Buffer> buffer;

void Gui::initialize()
{
/*
  buttonTexture = Texture::load("internal/textures/button");

  Environment::instance->guiMesh = std::make_shared<Mesh>();
  Vertex a;
  a.position = Vector3(0, 1, 0);
  a.texCoord = Vector3(0, 1, 0);
  Vertex b;
  b.position = Vector3(0, 0, 0);
  b.texCoord = Vector3(0, 0, 0);
  Vertex c;
  c.position = Vector3(1, 0, 0);
  c.texCoord = Vector3(1, 0, 0);
  Environment::instance->guiMesh->addFace(a, b, c);
  Vertex d;
  d.position = Vector3(1, 1, 0);
  d.texCoord = Vector3(1, 1, 0);
  Environment::instance->guiMesh->addFace(c, d, a);

  Environment::instance->guiMesh->generateVbos();
*/

  //std::sr1::shared_ptr<rend::Buffer> buffer =
  buffer =
    Environment::getContext()->createBuffer();

  buffer->add(rend::vec2(0, 0));
  buffer->add(rend::vec2(0, 1));
  buffer->add(rend::vec2(1, 1));
  buffer->add(rend::vec2(1, 1));
  buffer->add(rend::vec2(1, 0));
  buffer->add(rend::vec2(0, 0));

  std::sr1::shared_ptr<Shader> shader = Shader::load("shaders/gui");
  shader->internal->setAttribute("a_Position", buffer);

  Environment::instance->guiMaterial = std::sr1::make_shared<Material>();
  Environment::instance->guiMaterial->setShader(shader);

  Environment::instance->fontMaterial = std::sr1::make_shared<Material>();
  Environment::instance->fontMaterial->setShader(Shader::load("shaders/font"));
}

void Gui::applyProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, Environment::getScreenWidth(), Environment::getScreenHeight(),
    0, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void Gui::texture(Vector2 position, const std::sr1::observer_ptr<TextureAdapter>& texture)
{
  Gui::texture(Vector4(position.x, position.y,
    texture->getWidth(), texture->getHeight()), texture);
}

void Gui::texture(Vector4 position, const std::sr1::observer_ptr<TextureAdapter>& texture)
{
  Gui::texture(position, texture, std::sr1::shared_ptr<Material>());
}

void Gui::texture(Vector4 position, const std::sr1::observer_ptr<TextureAdapter>& texture, const std::sr1::shared_ptr<Material>& material)
{
  Mesh* mesh = Environment::instance->guiMesh.get();

  std::sr1::shared_ptr<Material> guiMaterial = Environment::instance->guiMaterial;

  if(material)
  {
    guiMaterial = material;
  }

  guiMaterial->setVariable("u_Projection",
    rend::ortho(0.0f, (float)Environment::getScreenWidth(),
    (float)Environment::getScreenHeight(), 0.0f, -1.0f, 1.0f));

  rend::mat4 m(1.0f);
  m = rend::translate(m, rend::vec3(position.x, position.y, 0));
  m = rend::scale(m, rend::vec3(position.z, position.w, 1));

  guiMaterial->setVariable("u_Model", m);
  guiMaterial->setVariable("u_Texture", texture);
  guiMaterial->apply();

  guiMaterial->shader->internal->setAttribute("a_Position", buffer);
  //guiMaterial->shader->internal->setAttribute("a_Position", mesh->positions);
  //guiMaterial->shader->internal->setAttribute("a_TexCoord", mesh->texCoords);
  guiMaterial->shader->internal->render();
}

/*
bool Gui::button(Vector4 position, std::string label)
{
  applyProjection();

  Mesh* mesh = Environment::instance->guiMesh.get();
  mesh->bind();
  glBindTexture(GL_TEXTURE_2D, buttonTexture->internal->getId());

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
*/

void Gui::text(Vector2 position, std::string label, Font* font)
{
  if(!font)
  {
    font = Font::load("fonts/army");
  }

  std::sr1::shared_ptr<Material> fontMaterial = Environment::instance->fontMaterial;
  fontMaterial->setVariable("u_Color", Vector4(1, 0, 0, 1));

  fontMaterial->setVariable("u_Projection",
    rend::ortho(0.0f, (float)Environment::getScreenWidth(),
    (float)Environment::getScreenHeight(), 0.0f, -1.0f, 1.0f));

  Vector2 size(font->getWidth(label), font->getHeight());
  rend::mat4 m(1.0f);
  m = rend::translate(m, rend::vec3(position.x, position.y + size.y, 0));

  if(label.length() > 0)
  {
    Glyph g = font->getGlyph(label.at(0));
    fontMaterial->setVariable("u_Texture", g.texture);

    for(size_t i = 0; i < label.length(); i++)
    {
      g = font->getGlyph(label.at(i));

      size = Vector2(font->getWidth(std::string("") + label.at(i)), font->getHeight());
      rend::mat4 gm = rend::scale(m, rend::vec3(size.x, -size.y, 1));

      fontMaterial->setVariable("u_View", rend::mat4(1.0f));
      fontMaterial->setVariable("u_Model", gm);
      fontMaterial->apply();

      fontMaterial->shader->internal->setAttribute("a_Position", g.mesh->positions);
      fontMaterial->shader->internal->setAttribute("a_TexCoord", g.mesh->texCoords);
      fontMaterial->shader->internal->render();

      m = rend::translate(m, rend::vec3(size.x + 1.0f, 0, 0));
    }
  }
}

}
