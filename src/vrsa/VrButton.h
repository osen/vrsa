#include <hydra/hydra.h>

using namespace hydra;

struct VrButton : public Component
{
  void setModel(std::sr1::observer_ptr<Model> model);
  void setTexture(std::sr1::observer_ptr<Texture> texture);
  void setBaseTexture(std::sr1::observer_ptr<Texture> texture);
  bool isClicked();
  bool isHover();
  void setLabel(const std::string& label);

  void onInitialize();
  void onTick();
  void onKill();

private:
  std::sr1::shared_ptr<Material> material;
  std::sr1::zero_initialized<bool> clicked;
  std::sr1::zero_initialized<bool> hover;
  std::sr1::zero_initialized<bool> down;
  std::sr1::zero_initialized<bool> startDown;
  std::sr1::zero_initialized<float> time;
  std::sr1::observer_ptr<ModelCollider> mc;

  std::sr1::observer_ptr<Entity> fe;
  std::sr1::observer_ptr<FontRenderer> fr;

};
