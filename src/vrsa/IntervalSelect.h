#include <hydra/hydra.h>

using namespace hydra;

struct VrButton;

struct IntervalSelect : public Component
{
  void onInitialize();
  void onTick();
  void onKill();

  int getSelected();

private:
  std::sr1::vector<std::sr1::observer_ptr<VrButton> > buttons;
  std::sr1::vector<std::sr1::observer_ptr<FontRenderer> > labels;
  std::sr1::observer_ptr<FontRenderer> label;
  std::sr1::zero_initialized<int> selected;

  void add(float x, float y, float r, std::sr1::observer_ptr<Texture> base, const std::string& label);

};
