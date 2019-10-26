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
  std::sr1::zero_initialized<bool> down;
  std::sr1::zero_initialized<bool> startDown;
  std::sr1::zero_initialized<float> time;
  std::sr1::observer_ptr<ModelCollider> mc;

};
