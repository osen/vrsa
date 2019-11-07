#include "VrManager.h"

void VrManager::setCameras(
  std::sr1::observer_ptr<Camera> leftCamera,
  std::sr1::observer_ptr<Camera> rightCamera)
{
  this->leftCamera = leftCamera;
  this->rightCamera = rightCamera;

  leftCamera->setRenderTarget(leftRt);
  rightCamera->setRenderTarget(rightRt);
}

void VrManager::onInitialize()
{
  getEntity()->addTag("vrmanager");
  getEntity()->setImmutable(true);

  font = Font::load("fonts/DroidWhite");

  leftRt = RenderTarget::create();
  rightRt = RenderTarget::create();

#ifdef ENABLE_VR

  std::cout << "*************************" << std::endl;
  std::cout << "Initializing VR Subsystem" << std::endl;
  std::cout << "*************************" << std::endl;

  Mouse::setLocked(true);
  ctx = ohmd_ctx_create();
  int num_devices = ohmd_ctx_probe(ctx);

  if(num_devices < 0)
  {
    printf("failed to probe devices: %s\n", ohmd_ctx_get_error(ctx));
    throw Exception("Failed to probe devices");
  }

  //std::cout << "HMD Devices: " << num_devices << std::endl;

  ohmd_device_settings* settings = ohmd_device_settings_create(ctx);
  int auto_update = 1;
  ohmd_device_settings_seti(settings, OHMD_IDS_AUTOMATIC_UPDATE, &auto_update);

  hmd = ohmd_list_open_device_s(ctx, 0, settings);

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

  //std::cout << "HMD Width: " << hmd_w << std::endl;
  //std::cout << "HMD Height: " << hmd_h << std::endl;

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

  leftLensCenter = Vector2(left_lens_center[0], left_lens_center[1]);
  rightLensCenter = Vector2(right_lens_center[0], right_lens_center[1]);

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

  warpMaterial = std::sr1::make_shared<Material>();
  warpMaterial->setShader(shader);

  //glUniform1i(glGetUniformLocation(shader, "warpTexture"), 0);
  //glUniform2fv(glGetUniformLocation(shader, "ViewportScale"), 1, viewport_scale);
  //glUniform3fv(glGetUniformLocation(shader, "aberr"), 1, aberr_scale);
  //glUniform1f(glGetUniformLocation(shader, "WarpScale"), warp_scale*warp_adj);
  //glUniform4fv(glGetUniformLocation(shader, "HmdWarpParam"), 1, distortion_coeffs);

  warpMaterial->setVariable("u_ViewportScale", Vector2(viewport_scale[0], viewport_scale[1]));
  warpMaterial->setVariable("u_Aberr", Vector3(aberr_scale[0], aberr_scale[1], aberr_scale[2]));

  warpMaterial->setVariable("u_WarpScale", warp_scale * warp_adj);

  warpMaterial->setVariable("u_HmdWarpParam",
    Vector4(distortion_coeffs[0], distortion_coeffs[1], distortion_coeffs[2], distortion_coeffs[3]));

  float oversampleScale = 2.0f;
  int eye_w = hmd_w / 2 * oversampleScale;
  int eye_h = hmd_h * oversampleScale;

  leftRt->setSize(eye_w, eye_h);
  rightRt->setSize(eye_w, eye_h);
  std::cout << "*************************" << std::endl;

#else

  leftRt->setSize(512, 512);
  rightRt->setSize(512, 512);

#endif
}

void VrManager::onTick()
{
#ifdef ENABLE_VR
  ohmd_ctx_update(ctx);
#endif

  if(Keyboard::getKeyDown('q'))
  {
    Environment::exit();
  }
}

void VrManager::onPreGui()
{
  Vector2 size(Environment::getScreenWidth(),
    Environment::getScreenHeight());

#ifdef ENABLE_VR
  size.x /= 2;
  warpMaterial->setVariable("u_LensCenter", leftLensCenter);
  Gui::texture(Vector4(0, 0, size.x, size.y), leftCamera->getRenderTarget(), warpMaterial);

  warpMaterial->setVariable("u_LensCenter", rightLensCenter);
  Gui::texture(Vector4(size.x, 0, size.x, size.y),
    rightCamera->getRenderTarget(), warpMaterial);

  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft]", font.get());

  mat4 vm = glm::inverse(getEntity()->getComponent<Transform>()->getModel());

  float matrix[16] = {0};
  ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX, matrix);
  mat4 proj = glm::make_mat4(matrix);
  rightCamera->setProjection(proj);
  ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_MODELVIEW_MATRIX, matrix);
  mat4 view = glm::make_mat4(matrix);
  rightCamera->setView(view * vm);

  ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_PROJECTION_MATRIX, matrix);
  proj = glm::make_mat4(matrix);
  leftCamera->setProjection(proj);
  ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_MODELVIEW_MATRIX, matrix);
  view = glm::make_mat4(matrix);
  leftCamera->setView(view * vm);
#else
  size.x -= 30;
  size.y -= 20;
  size.x /= 2;
  Gui::texture(Vector4(10, 10, size.x, size.y), leftCamera->getRenderTarget());
  Gui::texture(Vector4(10 + size.x + 10, 10, size.x, size.y),
    rightCamera->getRenderTarget());
#endif

}

void VrManager::onDoKill()
{
#ifdef ENABLE_VR
  if(ctx)
  {
    std::cout << ohmd_ctx_destroy << std::endl;
    ohmd_ctx_destroy(ctx);
    ctx = NULL;
  }
#endif
}

VrManager::~VrManager()
{
#ifdef ENABLE_VR
  if(ctx)
  {
    std::cout << ohmd_ctx_destroy << std::endl;
    ohmd_ctx_destroy(ctx);
    ctx = NULL;
  }
#endif
}

