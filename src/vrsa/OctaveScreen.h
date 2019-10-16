#include <hydra/hydra.h>

using namespace hydra;

struct OctaveConstruction;

struct OctaveScreen : public Component
{
  void onInitialize(const OctaveConstruction& oc);
};
