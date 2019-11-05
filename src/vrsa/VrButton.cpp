#include "VrButton.h"
#include "Pointer.h"

void VrButton::onInitialize()
{
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* model = Model::load("models/VrButton/VrButton");
  mr->setModel(model);
  material = mr->getMaterial();

  mc = getEntity()->addComponent<ModelCollider>();

  material->setShader(Shader::load("shaders/vrbutton"));

  setTexture(Texture::load("buttons/increase"));
}

void VrButton::onTick()
{
  bool last = down;
  down = false;
  clicked = false;

  time += Environment::getDeltaTime();
  Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());

  //Ray r = Environment::getCamera()->createRay(mpos);
  Entity* pe = Entity::findByTag("pointer");
  Pointer* pointer = pe->getComponent<Pointer>();
  Ray r = pointer->getRay();

  Vector3 hitLocal;
  Vector3 hitWorld;

  if(mc->colliding(r, hitLocal, hitWorld) == true)
  {
    pointer->restAgainst(getEntity()->getTransform()->getPosition());

    if(Mouse::getButtonDown(0) == true)
    {
      time = 1;
      startDown = true;
    }

    if(Mouse::getButton(0) == true && startDown == true)
    {
      down = true;
      material->setVariable("u_Interact", 1);
    }
    else
    {
      material->setVariable("u_Interact", 0.5f);
    }

    if(down == false && last == true)
    {
      startDown = false;
      clicked = true;
    }
  }
  else
  {
    time = 0;
    startDown = false;
    material->setVariable("u_Interact", 0);
  }

  material->setVariable("u_Time", time);
}

bool VrButton::isClicked()
{
  return clicked;
}

void VrButton::setTexture(std::sr1::observer_ptr<Texture> texture)
{
  material->setVariable("u_Glyph", texture);
}
