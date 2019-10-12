#include "VrManager.h"

void VrManager::onInitialize()
{
  font = Font::load("fonts/DroidWhite");
}

void VrManager::onGui()
{
  Gui::text(Vector2(10, 10), "Driver - OpenVR [simulator]", font.get());
}

