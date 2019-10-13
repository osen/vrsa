#include "VrManager.h"

void VrManager::onInitialize()
{
  font = Font::load("fonts/DroidWhite");
}

void VrManager::onGui()
{
  Gui::text(Vector2(10, 10), "Renderer: OpenGL [4.5 core]", font.get());
  Gui::text(Vector2(10, 40), "VR Driver: OpenVR [disconnected]", font.get());
  Gui::text(Vector2(10, 70), "Audio: OpenAL [soft, mono]", font.get());
}

