#include <hydra/hydra.h>

using namespace hydra;

struct OctaveScreen : public Component
{
  void onInitialize(int octaveIndex);
};
