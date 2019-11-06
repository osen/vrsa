#include <hydra/hydra.h>

using namespace hydra;

struct OctaveConstruction;
struct VrButton;
struct Octave;

struct OctaveScreen : public Component
{
  void onInitialize(const OctaveConstruction& oc);
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<VrButton> repeatButton;
  std::sr1::observer_ptr<VrButton> intervalsButton;
  std::sr1::observer_ptr<Octave> octave;

/*
  std::vector<int> playList;
  std::sr1::zero_initialized<float> timeout;
*/

};
