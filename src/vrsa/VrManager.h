#include <hydra/hydra.h>

using namespace hydra;

struct VrManager : public Component
{
  void onInitialize();
  void onTick();
  void onGui();

private:
  std::sr1::observer_ptr<Font> font;

};
