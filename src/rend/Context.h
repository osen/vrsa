#include <sr1/memory>

namespace rend
{

struct Texture;

struct Context
{
  static std::sr1::shared_ptr<Context> initialize(int argc, char* argv[]);

  std::sr1::shared_ptr<Texture> createTexture();

private:
  friend struct Texture;

  std::sr1::weak_ptr<Context> self;

  void pollForError();

};

}
