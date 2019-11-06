#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;

struct MenuScreen : public Component
{
  void onInitialize();
  void onTick();

  void setInterval(int interval);

private:
  std::sr1::observer_ptr<VrButton> quitButton;
  std::sr1::observer_ptr<VrButton> nextButton;
  std::sr1::observer_ptr<VrButton> questionButton;
  std::sr1::observer_ptr<VrButton> increaseButton;
  std::sr1::observer_ptr<VrButton> decreaseButton;

  std::sr1::observer_ptr<FontRenderer> frInterval;
  std::sr1::observer_ptr<FontRenderer> frName;

  std::sr1::zero_initialized<int> interval;

};
