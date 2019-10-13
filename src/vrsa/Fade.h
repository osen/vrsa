#include <hydra/hydra.h>

using namespace hydra;

struct Fade : public Component
{
  void onInitialize(const Vector3& c, bool fadeOut);
  void onTick();
  void onGui();

private:
  std::sr1::zero_initialized<float> amount;
  std::sr1::zero_initialized<bool> direction;
  Vector3 color;
};
