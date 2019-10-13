#include <hydra/hydra.h>

using namespace hydra;

struct ButtonBar : public Component
{
  void onInitialize(int mode);
  void onTick();

private:
  std::sr1::zero_initialized<int> mode;
  std::sr1::observer_ptr<ModelCollider> octaveCollider;
  std::sr1::observer_ptr<ModelCollider> exitCollider;
  std::sr1::observer_ptr<ModelCollider> pianoCollider;

};
