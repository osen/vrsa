#include <sr1/memory>

namespace rend
{

struct Texture;
struct Shader;

struct Context
{
  static std::sr1::shared_ptr<Context> initialize(int argc, char* argv[]);

  std::sr1::shared_ptr<Texture> createTexture();
  std::sr1::shared_ptr<Shader> createShader();

private:
  friend struct Texture;
  friend struct Shader;

  std::sr1::weak_ptr<Context> self;

  void pollForError();

};

}
