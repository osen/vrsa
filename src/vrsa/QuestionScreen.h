#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;
struct Octave;

struct QuestionScreen : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<Octave> octave;

  void playSound(vec3 pos);

};
