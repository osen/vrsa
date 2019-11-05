#include <hydra/hydra.h>

using namespace hydra;

struct Pointer : public Component
{
  void onInitialize(std::sr1::observer_ptr<Entity> parent);
  void onTick();
  void onGui();

  Ray getRay();
  void restAgainst(vec3 position);

private:
  std::sr1::zero_initialized<float> distance;
  std::sr1::zero_initialized<bool> collision;
  vec2 position;
  std::sr1::observer_ptr<Entity> parent;
  std::sr1::observer_ptr<ModelRenderer> mr;

};
