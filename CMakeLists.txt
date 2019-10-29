cmake_minimum_required(VERSION 2.6)
project(VRSA)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_definitions(-DSTD_SR1_DEBUG)

add_library(rend
  src/rend/Exception.cpp
  src/rend/Context.cpp
  src/rend/Texture.cpp
  src/rend/RenderTexture.cpp
  src/rend/Shader.cpp
  src/rend/Buffer.cpp
)
if(MSVC)
  target_link_libraries(rend glew32 opengl32 SDL2 glu32)
else()
  target_link_libraries(rend GLEW GL SDL2 GLU)
endif()

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
  src/hydra/Material.cpp
  src/hydra/Sound.cpp
  src/hydra/Texture.cpp
  src/hydra/RenderTarget.cpp
  src/hydra/Shader.cpp
  src/hydra/Transform.cpp
  src/hydra/Util.cpp
  src/hydra/World.cpp
  src/hydra/WorldCollider.cpp
  src/hydra/WorldRenderer.cpp
  src/hydra/lodepng.cpp
  src/hydra/tribox.cpp
)
if(MSVC)
  target_link_libraries(hydra rend openal32)
else()
  target_link_libraries(hydra rend openal)
endif()

add_executable(vrsa
  src/vrsa/main.cpp
  src/vrsa/IntroScreen.cpp
  src/vrsa/MainScreen.cpp
  src/vrsa/Player.cpp
  src/vrsa/Piano.cpp
  src/vrsa/Key.cpp
  src/vrsa/VrManager.cpp
  src/vrsa/ButtonBar.cpp
  src/vrsa/OctaveScreen.cpp
  src/vrsa/Octave.cpp
  src/vrsa/Fade.cpp
  src/vrsa/QuestionScreen.cpp
  src/vrsa/HelpScreen.cpp
  src/vrsa/KeyHelper.cpp
  src/vrsa/VrButton.cpp
  src/vrsa/MenuScreen.cpp
)
target_link_libraries(vrsa hydra openhmd hidapi)
