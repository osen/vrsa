#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;

struct MainScreen : public Component
{
  void onInitialize();
  void onTick();

private:
  std::sr1::observer_ptr<Model> world;
  std::sr1::observer_ptr<VrButton> quitButton;
  std::sr1::observer_ptr<VrButton> backButton;
  std::sr1::observer_ptr<VrButton> helpButton;
  std::sr1::observer_ptr<VrButton> examButton;

};
