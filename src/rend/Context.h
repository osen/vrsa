#include <sr1/memory>

namespace rend
{

struct Texture;
struct Shader;
struct Buffer;

struct Context
{
  static std::sr1::shared_ptr<Context> initialize(int argc, char* argv[]);

  std::sr1::shared_ptr<Texture> createTexture();
  std::sr1::shared_ptr<Shader> createShader();
  std::sr1::shared_ptr<Buffer> createBuffer();

private:
  friend struct Texture;
  friend struct Shader;
  friend struct Buffer;

  std::sr1::weak_ptr<Context> self;

  void pollForError();

};

}
