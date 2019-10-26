#include "VrButton.h"

void VrButton::onInitialize()
{
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* model = Model::load("models/VrButton/VrButton");
  mr->setModel(model);
  material = mr->getMaterial();

  material->setShader(Shader::load("shaders/vrbutton"));

  setTexture(Texture::load("buttons/increase"));
}

void VrButton::onTick()
{

}

void VrButton::setTexture(std::sr1::observer_ptr<Texture> texture)
{
  material->setVariable("u_Glyph", texture);
}
