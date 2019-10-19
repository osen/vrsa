#include <hydra/hydra.h>

using namespace hydra;

struct Player : public Component
{
  void onInitialize();
  void onTick();
  void onGui();

private:
  std::sr1::observer_ptr<Camera> camera;
  Vector2 lastMouse;

};
