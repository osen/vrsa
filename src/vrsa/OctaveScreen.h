#include <hydra/hydra.h>

using namespace hydra;

struct OctaveConstruction;
struct VrButton;

struct OctaveScreen : public Component
{
  void onInitialize(const OctaveConstruction& oc);
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;

};
