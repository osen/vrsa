#include <hydra/hydra.h>

using namespace hydra;

struct Player : public Component
{
  void onInitialize();

private:
  std::sr1::observer_ptr<Camera> camera;

};
