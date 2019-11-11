#include "QuestionScreen.h"
#include "Player.h"
#include "Fade.h"
#include "Octave.h"
#include "VrButton.h"
#include "MenuScreen.h"
#include "IntervalSelect.h"

#include <sstream>

#define REPEATS 2

void QuestionScreen::onInitialize()
{
  Environment::addEntity<Fade>(Vector3(0, 0, 0), true);
  Environment::addEntity<Player>();

  ModelRenderer* mr = getEntity()->addComponent<ModelRenderer>();
  Model* world = Model::load("models/MusicRoom/MusicRoom");
  mr->setModel(world);
  getEntity()->getComponent<Transform>()->setScale(Vector3(7, 7, 7));
  //getEntity()->getComponent<Transform>()->setPosition(Vector3(0, 0, -25));

  Entity* fe = Entity::create();
  fr = fe->addComponent<FontRenderer>();
  fr->setFont(Font::load("fonts/DroidWhiteLarge"));
  //fr->setMessage("Question: 1 / 24");
  fe->getComponent<Transform>()->setPosition(Vector3(0, 10, -25));
  //fe->getComponent<Transform>()->setRotation(Vector3(0, 180, 0));
  fe->getComponent<Transform>()->setScale(Vector3(0.1f, 0.1f, 0.1f));

  OctaveConstruction oc;
  oc.index = 48;
  octave = Environment::addEntity<Octave>(oc);
  octave->setReadOnly(true);

  backButton = Environment::addEntity<VrButton>();
  backButton->setTexture(Texture::load("buttons/exit"));
  backButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  backButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(-2.5, -2.5, -5));
  backButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  backButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));

  repeatButton = Environment::addEntity<VrButton>();
  repeatButton->setTexture(Texture::load("buttons/repeat"));
  repeatButton->getEntity()->getComponent<Transform>()->setScale(Vector3(2, 2, 2));
  repeatButton->getEntity()->getComponent<Transform>()->setPosition(Vector3(2.5f, -2.5, -5.0));
  repeatButton->getEntity()->getComponent<Transform>()->lookAt(Vector3(0, 0, 0));
  repeatButton->getEntity()->getComponent<Transform>()->rotate(Vector3(0, 180, 0));
  repeatButton->getEntity()->setEnabled(false);

  //timeout = 2;

  populateQuestions();

  currentQuestion = -1;
  nextQuestion();
}

int genrand(int min, int max);

void QuestionScreen::nextQuestion()
{
  repeats = 0;
  timeout = 1;

  currentQuestion++;
  Question q = questions.at(currentQuestion);

  int minIdx = 0;
  int maxIdx = 12 - q.interval - q.offset;

  int first = genrand(minIdx, maxIdx);
  int second = first + q.interval + q.offset;
  playlist.clear();
  playlist.push_back(first);
  playlist.push_back(second);

  q.first = first;
  q.second = second;
  questions.at(currentQuestion) = q;

  //std::cout << first << " " << second << " (" << q.interval << ", " << q.offset << ")" << std::endl;

  std::stringstream ss;
  ss << "Question: " << currentQuestion + 1 << " / " << questions.size();
  fr->setMessage(ss.str());
}

void QuestionScreen::endQuestions()
{
  Environment::clear();
  Environment::addEntity<MenuScreen>();
}

void QuestionScreen::onTick()
{
  if(backButton->isClicked())
  {
    Environment::clear();
    Environment::addEntity<MenuScreen>();
  }

  if(repeatButton->isClicked())
  {
    intervalSelect->getEntity()->kill();
    octave->setBackground(false);
    //octave->setPlaylist(playlist);
    questions.at(currentQuestion).repeats++;
    timeout = 1;
    repeatButton->getEntity()->setEnabled(false);
  }

  // If waiting for user to select answer, early out.
  if(intervalSelect)
  {
    int selected = intervalSelect->getSelected();

    if(selected == -1)
    {
      return;
    }

    intervalSelect->getEntity()->kill();
    octave->setBackground(false);

    if(currentQuestion == questions.size() - 1)
    {
      endQuestions();
    }
    else
    {
      nextQuestion();
    }

    return;
  }

  // If threshold then start playing.
  float pre = timeout;
  timeout -= Environment::getDeltaTime();
  if(pre > 0 && timeout <= 0)
  {
    octave->setPlaylist(playlist);
  }

  // If waiting for play timout, early exit.
  if(timeout > 0)
  {
    return;
  }

  // If playing finished, add interval screen.
  if(!octave->isPlaying())
  {
    octave->setBackground(true);
    intervalSelect = Environment::addEntity<IntervalSelect>();

    if(questions.at(currentQuestion).repeats < REPEATS)
    {
      repeatButton->getEntity()->setEnabled(true);
    }
  }
}

void QuestionScreen::populateQuestions()
{
  // Interval, start, offset

  questions.push_back(Question(3, 0));
  questions.push_back(Question(3, 1));
/*
  questions.push_back(Question(3, 2));
  questions.push_back(Question(3, -1));
  questions.push_back(Question(3, -2));

  questions.push_back(Question(3, 0));
  questions.push_back(Question(3, 1));
  questions.push_back(Question(3, 2));
  questions.push_back(Question(3, -1));
  questions.push_back(Question(3, -2));

  questions.push_back(Question(6, 0));
  questions.push_back(Question(6, 1));
  questions.push_back(Question(6, 2));
  questions.push_back(Question(6, -1));
  questions.push_back(Question(6, -2));

  questions.push_back(Question(6, 0));
  questions.push_back(Question(6, 1));
  questions.push_back(Question(6, 2));
  questions.push_back(Question(6, -1));
  questions.push_back(Question(6, -2));
*/
}

Question::Question(int interval, int offset)
{
  this->interval = interval;
  this->offset = offset;
}
