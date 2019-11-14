#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;

struct CompleteScreen : public Component
{
  void onInitialize(const std::string& id);
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> doneButton;

};
