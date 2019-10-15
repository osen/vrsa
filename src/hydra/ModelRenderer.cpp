#include "hydra.h"

#include <iostream>

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

void ModelRenderer::onInitialize()
{
  material = std::sr1::make_shared<Material>();
  material->setShader(Shader::load("shaders/default"));
}

std::sr1::shared_ptr<Material> ModelRenderer::getMaterial()
{
  return material;
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

void ModelRenderer::onRender()
{
  if(!Environment::getCamera())
  {
    //std::cout << "No camera" << std::endl;
    return;
  }

  //glPushMatrix();
  ////Environment::getCamera()->applyProjection();
  //Environment::getCamera()->applyView();

  Transform* t = getEntity()->getComponent<Transform>();
  //t->applyModel();

  mat4 projection = Environment::getCamera()->getProjection();
  mat4 view = Environment::getCamera()->getView();

  getMaterial()->setVariable("u_Projection", projection);
  getMaterial()->setVariable("u_View", view);

  mat4 modelMat = t->getModel();

  modelMat = rend::translate(modelMat, offset);
  modelMat = rend::translate(modelMat, -model->getCenter());

  //glTranslatef(
  //  offset.x,
  //  offset.y,
  //  offset.z);

  //glTranslatef(
  //  -model->getCenter().x,
  //  -model->getCenter().y,
  //  -model->getCenter().z);

  for(size_t pi = 0; pi < model->parts.size(); pi++)
  {
    //glPushMatrix();

    rend::mat4 partMat = rend::translate(modelMat, model->parts.at(pi)->offset);

    //glTranslatef(
    //  model->parts.at(pi)->offset.x,
    //  model->parts.at(pi)->offset.y,
    //  model->parts.at(pi)->offset.z);

    for(size_t ai = 0; ai < animations.size(); ai++)
    {
      // TODO; apply animation to modern matrix
      //animations.at(ai)->apply(model->parts.at(pi).get(), frame);
    }

    getMaterial()->setVariable("u_Model", partMat);

    for(size_t mgi = 0; mgi < model->parts.at(pi)->materialGroups.size(); mgi++)
    {
      std::shared_ptr<MaterialGroup> mg = model->parts.at(pi)->materialGroups.at(mgi);

      if(!mg->texture) continue;

      Mesh* mesh = mg->mesh.get();
      mesh->bind();
      glBindTexture(GL_TEXTURE_2D, model->parts.at(pi)->materialGroups.at(mgi)->texture->internal->getId());

      material->apply();
      material->shader->internal->setAttribute("a_Position", mesh->positions);
      material->shader->internal->setAttribute("a_TexCoord", mesh->texCoords);

      glUseProgram(material->shader->internal->getId());
      glDrawArrays(GL_TRIANGLES, 0, mesh->faces.size() * 3);
      glUseProgram(0);
    }

    //glPopMatrix();
  }

  //glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  //glPopMatrix();
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
