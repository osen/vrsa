#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;

struct QuestionScreen : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<VrButton> backButton;

};
