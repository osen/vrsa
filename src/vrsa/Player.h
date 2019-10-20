#include <hydra/hydra.h>

using namespace hydra;

struct Player : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<Camera> leftCamera;
  std::sr1::observer_ptr<Camera> rightCamera;
  Vector2 lastMouse;

};
