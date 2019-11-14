#include "VrButton.h"
#include "Pointer.h"

void VrButton::onInitialize()
{
  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();

  setModel(Model::load("models/VrButton/VrButton"));
  //Model* model = Model::load("models/VrButton/VrButton");
  //mr->setModel(model);

  mc = getEntity()->addComponent<ModelCollider>();
  material = mr->getMaterial();
  material->setShader(Shader::load("shaders/vrbutton"));

  setTexture(Texture::load("buttons/increase"));
}

void VrButton::setModel(std::sr1::observer_ptr<Model> model)
{
  ModelRenderer* mr = getEntity()->getComponent<ModelRenderer>();
  mr->setModel(model.get());
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
    hover = true;
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
    hover = false;
    time = 0;
    startDown = false;
    material->setVariable("u_Interact", 0);
  }

  material->setVariable("u_Time", time);
}

bool VrButton::isHover()
{
  return hover;
}

bool VrButton::isClicked()
{
  bool rtn = clicked;

  clicked = false;

  return rtn;
}

void VrButton::setBaseTexture(std::sr1::observer_ptr<Texture> texture)
{
  getEntity()->getComponent<ModelRenderer>()->setTexture(texture);
}

void VrButton::setTexture(std::sr1::observer_ptr<Texture> texture)
{
  material->setVariable("u_Glyph", texture);
}

void VrButton::onKill()
{
  if(fe)
  {
    fe->kill();
  }
}

void VrButton::setLabel(const std::string& label)
{
  if(!fr)
  {
    fe = Entity::create();
    fr = fe->addComponent<FontRenderer>();
    fr->setFont(Font::load("fonts/DroidWhiteLarge"));
    fe->getTransform()->setPosition(getEntity()->getTransform()->getPosition() + vec3(0, -0.5f, 0));
    fe->getTransform()->setRotation(getEntity()->getTransform()->getRotation());
    fe->getTransform()->moveForward(-1);
    fe->getTransform()->setRotation(getEntity()->getTransform()->getRotation() + vec3(-90, 0, 0));
    fe->getComponent<Transform>()->setScale(Vector3(0.01f, 0.01f, 1));
  }

  fr->setMessage(label);
}
