#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;
struct Octave;
struct IntervalSelect;

struct QuestionScreen : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<VrButton> repeatButton;
  std::sr1::observer_ptr<Octave> octave;
  std::sr1::observer_ptr<IntervalSelect> intervalSelect;
  std::sr1::zero_initialized<float> timeout;
  std::sr1::vector<int> playlist;

  void playSound(vec3 pos);

};
