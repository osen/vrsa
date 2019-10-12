#include <hydra/hydra.h>

using namespace hydra;

struct IntroScreen : public Component
{
  void onInitialize();
  void onTick();
  void onGui();

private:
  std::sr1::observer_ptr<Texture> logo;
  std::sr1::zero_initialized<float> timeout;

};

