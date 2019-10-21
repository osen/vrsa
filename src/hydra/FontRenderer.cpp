#include "hydra.h"

#include <GL/glew.h>

#include <iostream>

namespace hydra
{

void FontRenderer::onInitialize()
{
  material = std::sr1::make_shared<Material>();
  material->setShader(Shader::load("shaders/font"));

  scale = 1;
}

void FontRenderer::setScale(float scale)
{
  this->scale = scale;
}

void FontRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    std::cout << "No camera" << std::endl;
    return;
  }

  std::sr1::shared_ptr<RenderTarget> rt = Environment::getCamera()->getRenderTarget();

  mat4 projection = Environment::getCamera()->getProjection();
  material->setVariable("u_Projection", projection);

  mat4 view = Environment::getCamera()->getView();
  material->setVariable("u_View", view);

  Transform* t = getEntity()->getComponent<Transform>();
  mat4 modelMat = t->getModel();
  modelMat = rend::translate(modelMat, offset);
  //modelMat = rend::rotate(modelMat, rend::radians(180.0f), rend::vec3(0, 1, 0));

  Vector2 size(font->getWidth(message), font->getHeight());
  float sizeMod = scale;
  size *= sizeMod;
  modelMat = rend::translate(modelMat, rend::vec3(size.x / -2.0f, 0, 0));

  glFrontFace(GL_CW);

  if(message.length() > 0)
  {
    Glyph g = font->getGlyph(message.at(0));
    glBindTexture(GL_TEXTURE_2D, g.texture->internal->getId());

    for(size_t i = 0; i < message.length(); i++)
    {
      g = font->getGlyph(message.at(i));

      size = Vector2(font->getWidth(std::string("") + message.at(i)), font->getHeight());
      size *= sizeMod;

      material->setVariable("u_Texture", g.texture);
      material->setVariable("u_Color", Vector4(1, 1, 1, 1));

      rend::mat4 glyphMat = rend::scale(modelMat, rend::vec3(size.x, size.y, 1));
      material->setVariable("u_Model", glyphMat);
      material->apply();

      material->shader->internal->setAttribute("a_Position", g.mesh->positions);
      material->shader->internal->setAttribute("a_TexCoord", g.mesh->texCoords);

      if(rt)
      {
        material->shader->internal->render(rt->internal);
      }
      else
      {
        material->shader->internal->render();
      }

      modelMat = rend::translate(modelMat, rend::vec3(size.x + (1.0f * sizeMod), 0, 0));
    }
  }

  glFrontFace(GL_CCW);
}

void FontRenderer::setOffset(Vector3 offset)
{
  this->offset = offset;
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
