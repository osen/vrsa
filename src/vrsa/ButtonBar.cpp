#include "ButtonBar.h"
#include "MainScreen.h"
#include "QuestionScreen.h"
#include "HelpScreen.h"

void ButtonBar::onInitialize(int mode)
{
  this->mode = mode;

/*
  Entity* e = Entity::create();
  ModelRenderer* mr = e->addComponent<ModelRenderer>();
  mr->setModel(Model::load("models/ExitButton/ExitButton"));
  exitCollider = e->addComponent<ModelCollider>();
  e->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -5));
  e->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
*/

  Entity* e = NULL;
  ModelRenderer* mr = NULL;

  if(mode == 0)
  {
/*
    e = Entity::create();
    mr = e->addComponent<ModelRenderer>();
    mr->setModel(Model::load("models/ExamButton/ExamButton"));
    examCollider = e->addComponent<ModelCollider>();
    e->getComponent<Transform>()->setPosition(Vector3(2.5, -2.5, -5));
    e->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

    e = Entity::create();
    mr = e->addComponent<ModelRenderer>();
    mr->setModel(Model::load("models/HelpButton/HelpButton"));
    helpCollider = e->addComponent<ModelCollider>();
    e->getComponent<Transform>()->setPosition(Vector3(0, -2.5, -6));
    e->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
*/
  }

  if(mode == 1)
  {
    e = Entity::create();
    mr = e->addComponent<ModelRenderer>();
    mr->setModel(Model::load("models/PianoButton/PianoButton"));
    pianoCollider = e->addComponent<ModelCollider>();
    e->getComponent<Transform>()->setPosition(Vector3(0, -2.5, -5));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  }

  if(mode == 2 || mode == 1)
  {
    e = Entity::create();
    mr = e->addComponent<ModelRenderer>();
    mr->setModel(Model::load("models/BackButton/BackButton"));
    backCollider = e->addComponent<ModelCollider>();
    e->getComponent<Transform>()->setPosition(Vector3(2.5, -2.5, -5));
    e->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  }

  if(mode == 2)
  {
    e->getComponent<Transform>()->setPosition(Vector3(-1.5, -2.5, -5.40));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

    e = Entity::create();
    mr = e->addComponent<ModelRenderer>();
    mr->setModel(Model::load("models/NextButton/NextButton"));
    nextCollider = e->addComponent<ModelCollider>();
    e->getComponent<Transform>()->setPosition(Vector3(2.5, -2.5, -5));
    e->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
    e->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
    e->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  }
}

void ButtonBar::onTick()
{
  if(Mouse::getButtonDown(0) == true)
  {
    Vector2 mpos = Vector2(Mouse::getX(), Mouse::getY());
    Ray r = Environment::getCamera()->createRay(mpos);

    Vector3 hitLocal;
    Vector3 hitWorld;

/*
    if(exitCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::exit();
    }
*/

    if(pianoCollider && pianoCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::clear();
      Environment::addEntity<MainScreen>();
    }

/*
    if(examCollider && examCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::clear();
      Environment::addEntity<QuestionScreen>();
    }
*/

    if(backCollider && backCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::clear();
      Environment::addEntity<MainScreen>();
    }

    if(nextCollider && nextCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::clear();
      Environment::addEntity<QuestionScreen>();
    }

/*
    if(helpCollider && helpCollider->colliding(r, hitLocal, hitWorld) == true)
    {
      Environment::clear();
      Environment::addEntity<HelpScreen>();
    }
*/
  }
}
