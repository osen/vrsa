#include <hydra/hydra.h>

#ifdef ENABLE_VR
  #define OHMD_STATIC
  #include <openhmd.h>
#endif

using namespace hydra;

struct VrManager : public Component
{
  void onInitialize(
    std::sr1::observer_ptr<Camera> leftCamera,
    std::sr1::observer_ptr<Camera> rightCamera);

  void onTick();
  void onGui();
  void onDoKill();

  ~VrManager();

private:
  std::sr1::observer_ptr<Font> font;
  std::sr1::observer_ptr<Camera> leftCamera;
  std::sr1::observer_ptr<Camera> rightCamera;

  std::sr1::shared_ptr<RenderTarget> leftRt;
  std::sr1::shared_ptr<RenderTarget> rightRt;
  Vector2 leftLensCenter;
  Vector2 rightLensCenter;

  std::sr1::shared_ptr<Material> warpMaterial;
  std::sr1::zero_initialized<bool> disableWarp;

#ifdef ENABLE_VR
  std::sr1::zero_initialized<ohmd_context*> ctx;
  std::sr1::zero_initialized<ohmd_device*> hmd;
#endif
};
