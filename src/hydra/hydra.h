#ifndef HYDRA_HYDRA_H
#define HYDRA_HYDRA_H

#include <glm/glm.hpp>

#ifdef OPENGLD
  #include <GL/gl.h>
#else
  #include <GL/glew.h>
#endif

#include <AL/al.h>
#include <AL/alc.h>

#include <sr1/noncopyable>
#include <sr1/zero_initialized>
#include <sr1/memory>

#include <vector>
#include <string>
#include <memory>
#include <ctime>

namespace hydra
{

typedef glm::mat4 Matrix;
typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;

struct Part;
struct ColliderColumn;
struct DataStore;
struct Gui;
class WorldRenderer;
class WorldCollider;
class ModelCollider;
class Device;
class World;
class Entity;
class Environment;
class Model;
class ModelRenderer;
class FontRenderer;
class Transform;
class Sound;

struct Extent
{
  Vector3 max;
  Vector3 min;
};

struct Vertex
{
  Vector3 position;
  Vector2 texCoord;
  Vector3 normal;
};

struct Face
{
  Vertex a;
  Vertex b;
  Vertex c;
};

struct Ray
{
  Vector3 origin;
  Vector3 direction;
};

class Texture : public std::sr1::noncopyable, public std::sr1::enable_observer
{
  friend class hydra::WorldRenderer;
  friend class hydra::ModelRenderer;
  friend class hydra::FontRenderer;
  friend struct hydra::Gui;

  std::sr1::zero_initialized<int> width;
  std::sr1::zero_initialized<int> height;
  std::vector<unsigned char> data;
  std::sr1::zero_initialized<GLuint> id;
  std::string path;

public:
  static Texture* load(std::string path);
  Texture();
  Texture(Vector4 rgba);
  ~Texture();

  std::string getPath();
  Vector4 getPixel(int x, int y);
  int getWidth();
  int getHeight();
};

class Sound
{
  friend class hydra::Environment;

  ALuint id;
  std::string path;

public:
  static Sound* load(std::string path);
  ~Sound();

  void play();
  void play(float vol, float varMin, float varMax);

};

class Mesh : public std::sr1::noncopyable
{
  friend class hydra::Model;
  friend class hydra::ModelRenderer;
  friend class hydra::World;
  friend class hydra::WorldRenderer;
  friend class hydra::WorldCollider;
  friend struct hydra::Gui;
  friend class hydra::FontRenderer;
  friend class hydra::ModelCollider;

  std::vector<Face> faces;
  std::sr1::zero_initialized<GLuint> buffer;

public:
  ~Mesh();
  Face getFace(size_t i);
  void addFace(Vector3 a, Vector3 b, Vector3 c);
  void addFace(Vertex a, Vertex b, Vertex c);
  void generateVbos();
  void bind();

};

struct MaterialGroup
{
  std::sr1::observer_ptr<Texture> texture;
  std::shared_ptr<Mesh> mesh;

  MaterialGroup()
  {
    mesh.reset(new Mesh());
  }
};

class Component : public std::sr1::enable_observer
{
  friend class hydra::Entity;

  std::sr1::observer_ptr<Entity> entity;
  std::sr1::zero_initialized<bool> began;
  std::sr1::zero_initialized<bool> disabled;

  virtual void onInitialize();
  virtual void onBegin();
  virtual void onTick();
  virtual void onRender();
  virtual void onPostRender();
  virtual void onGui();
  virtual void onKill();

public:
  virtual ~Component();

  Entity* getEntity();
  void setEnabled(bool enabled);
  bool getEnabled();

};

struct PartTransform
{
  std::string partName;
  Vector3 position;
  Vector3 rotation;
};

struct Frame
{
  std::vector<PartTransform> transforms;
};

class Animation
{
  friend class hydra::ModelRenderer;

  std::string path;
  std::vector<Frame> frames;
  std::sr1::zero_initialized<double> fps;

public:
  static Animation* load(std::string path);
  void apply(Part* part, double frame);
  void setFps(double fps);

};

struct Part : public std::sr1::enable_observer
{
  std::vector<std::shared_ptr<MaterialGroup> > materialGroups;
  std::sr1::observer_ptr<Texture> texture;
  Vector3 offset;
  Vector3 size;
  std::string name;

};

class Model : public std::sr1::enable_observer
{
  friend class hydra::ModelRenderer;
  friend class hydra::ModelCollider;

  std::string path;
  std::vector<std::shared_ptr<Part> > parts;
  std::vector<std::shared_ptr<Texture> > textures;
  Vector3 size;
  Vector3 center;

  void processFLine(std::vector<std::string>& splitLine, DataStore& store);
  void processOLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVnLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVtLine(std::vector<std::string>& splitLine, DataStore& store);
  void processMtlLibLine(std::vector<std::string>& splitLine, DataStore& store);
  void processUsemtlLine(std::vector<std::string>& splitLine, DataStore& store);
  void generateVbos();
  void generateOffsets();

public:
  static Model* load(std::string path);
  Vector3 getSize();
  Vector3 getCenter();

};

struct Glyph
{
  char c;
  int x;
  int y;
  int width;
  int height;
  std::sr1::observer_ptr<Texture> texture;
  std::shared_ptr<Mesh> mesh;
};

class Font : public std::sr1::enable_observer
{
  friend class hydra::FontRenderer;
  friend struct hydra::Gui;

  std::shared_ptr<Mesh> mesh;
  std::sr1::observer_ptr<Texture> texture;
  std::vector<Glyph> glyphs;
  std::string path;

  bool dirtyColumn(int x);
  void generateGlyphs();

public:
  static Font* load(std::string path);
  Glyph getGlyph(char c);
  int getWidth(std::string sample);
  int getHeight();

};

class ModelRenderer : public Component
{
  std::sr1::observer_ptr<Model> model;
  std::vector<Animation*> animations;
  std::sr1::zero_initialized<double> frame;
  Vector2 screenCoordinate;
  Vector3 offset;

  virtual void onRender();
  virtual void onTick();
  void updateScreenPosition(Transform* cameraTransform);

public:
  void setOffset(Vector3 offset);
  Vector3 getOffset();
  void setModel(Model* model);
  Model* getModel();
  void attachAnimation(Animation* animation);
  void detachAnimation(Animation* animation);

  Vector2 getScreenCoordinate();

};

class FontRenderer : public Component
{
  std::sr1::observer_ptr<Font> font;
  std::string message;

  virtual void onPostRender();

public:
  void setFont(Font *font);
  void setMessage(std::string message);

};

struct EntitySpawnInfo
{
  std::string type;
  std::sr1::zero_initialized<bool> positionSet;
  Vector3 position;
};

struct MaterialInfo
{
  std::string name;
  std::string texturePath;
};

struct DataStore
{
  std::vector<Vector3> positions;
  std::vector<Vector2> texCoords;
  std::vector<Vector3> normals;
  std::sr1::observer_ptr<Part> currentPart;
  std::shared_ptr<MaterialGroup> currentMg;
  std::string nextPartName;
  std::shared_ptr<EntitySpawnInfo> currentEntity;
  std::vector<MaterialInfo> materialInfos;
  std::vector<std::string> tokens;
};

class Collider : public Component
{
  Vector3 size;
  Vector3 offset;
  Vector3 lastPosition;

public:
  virtual void onBegin();
  virtual void onTick();
  void setSize(Vector3 size);
  void setOffset(Vector3 size);

};

class ModelCollider : public Component
{
public:
  bool colliding(Ray& ray, Vector3 &hitLocal, Vector3 &hitWorld);

};

class WorldCollider : public Component
{
  std::vector<std::shared_ptr<ColliderColumn> > cols;
  std::sr1::observer_ptr<World> world;
  Extent extent;

  void generateExtent();
  void addFace(Face face);

public:
  void onInitialize();
  Extent getExtent();
  bool isColliding(Vector3 position, Vector3 size);
  Vector3 getCollisionResponse(Vector3 position, Vector3 size, bool& solved, Vector3 lastPosition);

};

class World : public Component
{
  friend class hydra::WorldRenderer;
  friend class hydra::WorldCollider;

  std::string path;
  std::vector<std::shared_ptr<MaterialGroup> > materialGroups;
  std::vector<std::shared_ptr<Texture> > textures;
  std::vector<std::shared_ptr<EntitySpawnInfo> > entitySpawnInfos;

  void processMtlLibLine(std::vector<std::string>& splitLine, DataStore& store);
  void processUsemtlLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVtLine(std::vector<std::string>& splitLine, DataStore& store);
  void processVnLine(std::vector<std::string>& splitLine, DataStore& store);
  void processOLine(std::vector<std::string>& splitLine, DataStore& store);
  void processFLine(std::vector<std::string>& splitLine, DataStore& store);
  void generateVbos();

public:
  void onInitialize(std::string path);
};

class WorldRenderer : public Component
{
  std::sr1::observer_ptr<World> world;

  virtual void onRender();

public:
  void onInitialize();

};

class Camera : public Component
{
  static Vector4 clearColor;

  std::sr1::zero_initialized<int> id;
  Vector3 smoothPosition;
  Vector3 offset;

public:
  void applyProjection();
  void applyView();
  void setOffset(Vector3 offset);
  virtual void onInitialize();
  virtual void onRender();
  Ray createRay(Vector2 screenPosition);

  static void setClearColor(Vector4 clearColor);
  static Vector4 getClearColor();
};

struct ColliderType
{
public:
  static const int NONE = 0;
  static const int UNKNOWN = 1;
  static const int SPHERE = 2;
  static const int CAPSULE = 3;
};

class Transform : public Component
{
  friend class hydra::World;

  Vector3 position;
  Vector3 lastPosition;
  Vector3 rotation;
  Vector3 scale;

public:
  void setPosition(Vector3 position);
  void setRotation(Vector3 rotation);
  Vector3 getPosition();
  Vector3 getRotation();
  void rotate(Vector3 rotation);
  void setScale(Vector3 scale);
  Vector3 getScale();

  Vector3 getForward();
  Vector3 getRight();
  Vector3 getRelative(Vector3 position);

  //simple Vector3 lerp without collision checking
  void moveTowards(Vector3 target, float speed);
  void moveForward(float speed);
  void moveBackward(float speed);
  void moveRight(float speed);
  void moveLeft(float speed);
  void move(Vector3 direction, float speed);
  
  void lookAt(Vector3 target);

  void applyModel();

  virtual void onInitialize();
  virtual void onKill();
  virtual void onTick();
};

class Entity : public std::sr1::enable_observer
{
  friend class hydra::Environment;

  std::vector<std::string> tags;
  std::vector<std::shared_ptr<Component> > components;
  std::sr1::zero_initialized<bool> alive;
  std::sr1::observer_ptr<Transform> transform;
  std::sr1::observer_ptr<Entity> self;

  void tick();
  void render();
  void postRender();
  void gui();
  void _kill();

public:
  static Entity* create();
  static Entity* createBox();
  static Entity* findByTag(std::string tag);
  static void findByTag(std::string tag, std::vector<Entity*>& output);
  void kill();
  void addTag(std::string tag);
  Transform* getTransform();
  bool hasTag(std::string tag);

  template <typename T>
  T* addComponent()
  {
    std::shared_ptr<T> rtn = std::make_shared<T>();
    //std::shared_ptr<Component> c = std::static_pointer_cast<Component>(rtn);
    rtn->entity = self;
    components.push_back(rtn);
    rtn->onInitialize();

    return rtn.get();
  }

  template <typename T, typename A>
  T* addComponent(A a)
  {
    std::shared_ptr<T> rtn = std::make_shared<T>();
    rtn->entity = self;
    components.push_back(rtn);
    rtn->onInitialize(a);

    return rtn.get();
  }

  template <typename T>
  T* getComponent()
  {
    for(size_t i = 0; i < components.size(); i++)
    {
      std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(components.at(i));

      if(c.get())
      {
        return c.get();
      }
    }

    return NULL;
  }
};

struct RegisterAssociation
{
  char type[256];
  void (*func)(Entity*);
};

class Mouse
{
public:
  static void motion(int x, int y);
  static void mouse(int button, int state, int x, int y);

  static int getX();
  static int getY();
  static bool getButton(int button = 0);
  static bool getButtonDown(int button = 0);

  static int x;
  static int y;
  static bool button0;
  static bool button0Down;
};

class Keyboard
{
public:
  static void keyboard(unsigned char key, int x, int y);
  static void keyboardUp(unsigned char key, int x, int y);
  static void keyboardSpecial(int key, int x, int y);
  static void keyboardSpecialUp(int key, int x, int y);

  static bool getKey(unsigned char key);
  static bool getKeyDown(unsigned char key);
  //static bool getKeyUp(unsigned char key);
};

class Environment
{
  friend class hydra::Entity;
  friend class hydra::World;
  friend class hydra::Texture;
  friend class hydra::Keyboard;
  friend class hydra::Model;
  friend struct hydra::Gui;
  friend class hydra::Font;
  friend class hydra::Animation;
  friend class hydra::Sound;
  friend class hydra::Transform;

  static std::shared_ptr<Environment> instance;
  static RegisterAssociation registrations[256];

  std::vector<std::shared_ptr<Entity> > entities;
  std::vector<std::shared_ptr<Model> > models;
  std::vector<std::shared_ptr<Font> > fonts;
  std::vector<std::shared_ptr<Animation> > animations;
  std::vector<std::shared_ptr<Texture> > textures;
  std::vector<std::shared_ptr<Sound> > sounds;
  std::vector<std::shared_ptr<World> > worlds;

  std::vector<unsigned char> keys;
  std::vector<unsigned char> downKeys;
  std::sr1::zero_initialized<float> deltaTime;
  std::sr1::zero_initialized<float> lastTime;
  std::sr1::zero_initialized<time_t> startTime;
  std::sr1::zero_initialized<int> screenWidth;
  std::sr1::zero_initialized<int> screenHeight;
  std::sr1::zero_initialized<Camera*> camera;
  std::string prefix;
  std::string assetsDirectory;

  ALCdevice* device;
  ALCcontext* context;
  std::vector<ALuint> audioSources;

  std::shared_ptr<Mesh> guiMesh;

  static void idle();
  static void display();
  //static void _keyboard(unsigned char key, int x, int y);
  //static void keyboardUp(unsigned char key, int x, int y);
  static void reshape(int width, int height);
  static void updateDeltaTime();
  void setupPaths(char* argv0);

  void openAudio();
  void closeAudio();

  static void initializePre(int argc, char *argv[]);
  static void initializePost();

public:
  template <typename T>
  static void initialize(int argc, char *argv[])
  {
    initializePre(argc, argv);
    addEntity<T>();
    initializePost();
  }

  static void stopAudio();
  static void exit();
  static void repaint();
  static float getDeltaTime();
  static time_t getLifeTime();
  static void registerType(std::string type, void (*attachFunc)(Entity*));
  static int getScreenWidth();
  static int getScreenHeight();
  static void setCamera(Camera* camera);
  static Camera* getCamera();
  static void setTitle(std::string title);
  static Entity* spawn(std::string name, Vector3 position, Vector3 rotation);
  static std::string getAssetsDirectory();
  static void clear();

  template <typename T>
  static T* addEntity()
  {
    Entity* e = Entity::create();
    T* t = e->addComponent<T>();
    return t;
  }

  template <typename T, typename A>
  static T* addEntity(A a)
  {
    Entity* e = Entity::create();
    T* t = e->addComponent<T>(a);
    return t;
  }
};

struct Gui
{
  static Texture* buttonTexture;
  static void initialize();

  static void applyProjection();
  static void texture(Vector4 position, Texture* texture);
  static void texture(Vector2 position, Texture* texture);
  static bool button(Vector4 position, std::string label);
  static void text(Vector2 position, std::string label, Font* font = NULL);
};

struct Util
{
  static bool rectIntersect(Vector4 a, Vector4 b);
  static void splitStringWhitespace(std::string& input, std::vector<std::string>& output);
  static void splitString(std::string& input, char splitter, std::vector<std::string>& output);
  static int randBetween(int min, int max);
};

class Exception : public std::exception
{
public:
  Exception(std::string message)
  {
    this->message = message;
  }

  virtual ~Exception() throw() { }

  virtual const char* what() const throw()
  {
    return message.c_str();
  }

private:
  std::string message;

};

}

#define HYDRA_REGISTER(T) \
  void _##T##Attach(hydra::Entity* ent) \
  { \
    ent->addComponent<T>(); \
  } \
  \
  class _##T##Register \
  { \
  public: \
    _##T##Register() \
    { \
      hydra::Environment::registerType(#T, _##T##Attach); \
    } \
  }; \
  _##T##Register _##T##RegisterInstance;

#endif
