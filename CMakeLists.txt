cmake_minimum_required(VERSION 2.6)
project(VRSA)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_library(hydra
  src/hydra/Mouse.cpp
  src/hydra/Animation.cpp
  src/hydra/Camera.cpp
  src/hydra/Collider.cpp
  src/hydra/Component.cpp
  src/hydra/Entity.cpp
  src/hydra/Environment.cpp
  src/hydra/Font.cpp
  src/hydra/FontRenderer.cpp
  src/hydra/Gui.cpp
  src/hydra/Keyboard.cpp
  src/hydra/Model.cpp
  src/hydra/ModelRenderer.cpp
  src/hydra/Sound.cpp
  src/hydra/Texture.cpp
  src/hydra/Transform.cpp
  src/hydra/Util.cpp
  src/hydra/World.cpp
  src/hydra/WorldCollider.cpp
  src/hydra/WorldRenderer.cpp
  src/hydra/lodepng.cpp
  src/hydra/tribox.cpp
)
target_link_libraries(hydra GLEW GL openal glut GLU ogg vorbis vorbisfile)

add_executable(vrsa
  src/vrsa/main.cpp
  src/vrsa/IntroScreen.cpp
  src/vrsa/MainScreen.cpp
  src/vrsa/Player.cpp
  src/vrsa/Piano.cpp
  src/vrsa/Key.cpp
  src/vrsa/VrManager.cpp
)
target_link_libraries(vrsa hydra)
