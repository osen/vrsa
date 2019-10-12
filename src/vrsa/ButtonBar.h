#include <hydra/hydra.h>

using namespace hydra;

struct ButtonBar : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<ModelCollider> octaveCollider;

};
