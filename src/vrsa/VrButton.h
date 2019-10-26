#include <hydra/hydra.h>

using namespace hydra;

struct VrButton : public Component
{

  void setTexture(std::sr1::observer_ptr<Texture> texture);
  bool isClicked();

  void onInitialize();
  void onTick();

private:
  std::sr1::shared_ptr<Material> material;
  std::sr1::zero_initialized<bool> clicked;

};
