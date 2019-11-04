#include <hydra/hydra.h>

using namespace hydra;

struct Pointer : public Component
{
  void onInitialize(std::sr1::observer_ptr<Entity> parent);
  void onTick();
  Ray getRay();

private:
  vec2 position;
  std::sr1::observer_ptr<Entity> parent;

};
