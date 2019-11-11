#include <hydra/hydra.h>

using namespace hydra;

struct OctaveConstruction;
struct VrButton;
struct Octave;
struct IntervalSelect;

struct OctaveScreen : public Component
{
  void onInitialize(const OctaveConstruction& oc);
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<VrButton> repeatButton;
  std::sr1::observer_ptr<VrButton> intervalsButton;
  std::sr1::observer_ptr<Octave> octave;
  std::sr1::observer_ptr<IntervalSelect> intervalSelect;
  std::sr1::vector<int> playlist;

};
