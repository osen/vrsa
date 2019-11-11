#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;
struct Octave;
struct IntervalSelect;

struct Question
{
  std::sr1::zero_initialized<int> interval;
  std::sr1::zero_initialized<int> offset;
  std::sr1::zero_initialized<int> first;
  std::sr1::zero_initialized<int> second;
  std::sr1::zero_initialized<int> repeats;
  std::sr1::zero_initialized<int> answer;

  Question(int interval, int offset);

};

struct QuestionScreen : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<FontRenderer> fr;
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<VrButton> repeatButton;
  std::sr1::observer_ptr<Octave> octave;
  std::sr1::observer_ptr<IntervalSelect> intervalSelect;
  std::sr1::zero_initialized<float> timeout;
  std::sr1::vector<int> playlist;

  std::sr1::zero_initialized<int> currentQuestion;
  std::sr1::zero_initialized<int> repeats;

  void populateQuestions();
  void nextQuestion();
  void endQuestions();

  std::sr1::vector<Question> questions;
};
