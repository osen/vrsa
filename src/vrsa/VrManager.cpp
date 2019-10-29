#include "VrManager.h"

void VrManager::onInitialize(
  std::sr1::observer_ptr<Camera> leftCamera,
  std::sr1::observer_ptr<Camera> rightCamera)
{
  font = Font::load("fonts/DroidWhite");

  this->leftCamera = leftCamera;
  this->rightCamera = rightCamera;

  ctx = ohmd_ctx_create();
  int num_devices = ohmd_ctx_probe(ctx);

  if(num_devices < 0)
  {
    printf("failed to probe devices: %s\n", ohmd_ctx_get_error(ctx));
    throw Exception("Failed to probe devices");
  }

  std::cout << "HMD Devices: " << num_devices << std::endl;

  ohmd_device_settings* settings = ohmd_device_settings_create(ctx);
  int auto_update = 1;
  ohmd_device_settings_seti(settings, OHMD_IDS_AUTOMATIC_UPDATE, &auto_update);

  ohmd_device* hmd = ohmd_list_open_device_s(ctx, 0, settings);

  if(!hmd)
  {
    printf("failed to open device: %s\n", ohmd_ctx_get_error(ctx));
    throw Exception("Failed to open device");
  }

  int hmd_w = 0;
  int hmd_h = 0;
  ohmd_device_geti(hmd, OHMD_SCREEN_HORIZONTAL_RESOLUTION, &hmd_w);
  ohmd_device_geti(hmd, OHMD_SCREEN_VERTICAL_RESOLUTION, &hmd_h);
  float ipd = 0;
  ohmd_device_getf(hmd, OHMD_EYE_IPD, &ipd);

  std::cout << "HMD Width: " << hmd_w << std::endl;
  std::cout << "HMD Height: " << hmd_h << std::endl;

  float viewport_scale[2] = {0};
  float distortion_coeffs[4] = {0};
  float aberr_scale[3] = {0};
  float sep = 0;
  float left_lens_center[2] = {0};
  float right_lens_center[2] = {0};

  //viewport is half the screen
  ohmd_device_getf(hmd, OHMD_SCREEN_HORIZONTAL_SIZE, &(viewport_scale[0]));
  viewport_scale[0] /= 2.0f;
  ohmd_device_getf(hmd, OHMD_SCREEN_VERTICAL_SIZE, &(viewport_scale[1]));

  //distortion coefficients
  ohmd_device_getf(hmd, OHMD_UNIVERSAL_DISTORTION_K, &(distortion_coeffs[0]));
  ohmd_device_getf(hmd, OHMD_UNIVERSAL_ABERRATION_K, &(aberr_scale[0]));

  //calculate lens centers (assuming the eye separation is the distance between the lens centers)
  ohmd_device_getf(hmd, OHMD_LENS_HORIZONTAL_SEPARATION, &sep);
  ohmd_device_getf(hmd, OHMD_LENS_VERTICAL_POSITION, &(left_lens_center[1]));
  ohmd_device_getf(hmd, OHMD_LENS_VERTICAL_POSITION, &(right_lens_center[1]));
  left_lens_center[0] = viewport_scale[0] - sep/2.0f;
  right_lens_center[0] = sep/2.0f;

  //assume calibration was for lens view to which ever edge of screen is further away from lens center
  float warp_scale = (left_lens_center[0] > right_lens_center[0]) ?
    left_lens_center[0] : right_lens_center[0];

  float warp_adj = 1.0f;
  ohmd_device_settings_destroy(settings);

  // Use shader from file (better style, more consistent).
  //const char* vertex = NULL;
  //ohmd_gets(OHMD_GLSL_ES_DISTORTION_VERT_SRC, &vertex);
  //const char* fragment = NULL;
  //ohmd_gets(OHMD_GLSL_ES_DISTORTION_FRAG_SRC, &fragment);
  //std::cout << vertex << std::endl;
  //std::cout << "---------------------------------" << std::endl;
  //std::cout << fragment << std::endl;

  std::sr1::shared_ptr<Shader> shader = Shader::load("shaders/openhmd");

  std::sr1::shared_ptr<Material> material = std::sr1::make_shared<Material>();
  material->setShader(shader);

  //glUniform1i(glGetUniformLocation(shader, "warpTexture"), 0);
  //glUniform2fv(glGetUniformLocation(shader, "ViewportScale"), 1, viewport_scale);
  //glUniform3fv(glGetUniformLocation(shader, "aberr"), 1, aberr_scale);

  material->setVariable("u_ViewportScale", Vector2(viewport_scale[0], viewport_scale[1]));
  material->setVariable("u_Aberr", Vector3(aberr_scale[0], aberr_scale[1], aberr_scale[2]));

  float oversampleScale = 2.0f;
  int eye_w = hmd_w / 2 * oversampleScale;
  int eye_h = hmd_h * oversampleScale;

  leftRt = RenderTarget::create();
  rightRt = RenderTarget::create();
  leftRt->setSize(eye_w, eye_h);
  rightRt->setSize(eye_w, eye_h);
}

void VrManager::onTick()
{
  ohmd_ctx_update(ctx);

  if(Keyboard::getKeyDown('v'))
  {
    if(leftCamera->getRenderTarget())
    {
      leftCamera->setRenderTarget(std::sr1::shared_ptr<RenderTarget>());
      rightCamera->setRenderTarget(std::sr1::shared_ptr<RenderTarget>());
    }
    else
    {
      leftCamera->setRenderTarget(leftRt);
      rightCamera->setRenderTarget(rightRt);
    }
  }
}

void VrManager::onGui()
{
  if(leftCamera->getRenderTarget())
  {
    Vector2 size(Environment::getScreenWidth(),
      Environment::getScreenHeight());

    size.x -= 30;
    size.x /= 2;

    size.y -= 20;

    Gui::texture(Vector4(10, 10, size.x, size.y), leftCamera->getRenderTarget());

    Gui::texture(Vector4(10 + size.x + 10, 10, size.x, size.y),
      rightCamera->getRenderTarget());
  }

  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft, mono]", font.get());
}

void VrManager::onKill()
{
  //std::cout << "Destroying" << std::endl;
  ohmd_ctx_destroy(ctx);
}
