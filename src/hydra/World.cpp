#include "hydra.h"

#include <fstream>
#include <iostream>

#include <string.h>

#define BUFFER_OFFSET(bytes) ((GLvoid*)(sizeof(GLfloat) * bytes))

namespace hydra
{

void Mesh::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, positions->getId());
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, texCoords->getId());
  glTexCoordPointer(2, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, normals->getId());
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateVbos()
{
  std::vector<GLfloat> data;

  positions = Environment::getContext()->createBuffer();
  texCoords = Environment::getContext()->createBuffer();
  normals = Environment::getContext()->createBuffer();

  for(size_t f = 0; f < faces.size(); f++)
  {
    Face face = faces.at(f);

    positions->add(face.a.position);
    texCoords->add(face.a.texCoord);
    normals->add(face.a.normal);

    positions->add(face.b.position);
    texCoords->add(face.b.texCoord);
    normals->add(face.b.normal);

    positions->add(face.c.position);
    texCoords->add(face.c.texCoord);
    normals->add(face.c.normal);

    data.push_back(face.a.position.x);
    data.push_back(face.a.position.y);
    data.push_back(face.a.position.z);
    data.push_back(face.a.texCoord.x);
    data.push_back(face.a.texCoord.y);
    data.push_back(face.a.normal.x);
    data.push_back(face.a.normal.y);
    data.push_back(face.a.normal.z);

    data.push_back(face.b.position.x);
    data.push_back(face.b.position.y);
    data.push_back(face.b.position.z);
    data.push_back(face.b.texCoord.x);
    data.push_back(face.b.texCoord.y);
    data.push_back(face.b.normal.x);
    data.push_back(face.b.normal.y);
    data.push_back(face.b.normal.z);

    data.push_back(face.c.position.x);
    data.push_back(face.c.position.y);
    data.push_back(face.c.position.z);
    data.push_back(face.c.texCoord.x);
    data.push_back(face.c.texCoord.y);
    data.push_back(face.c.normal.x);
    data.push_back(face.c.normal.y);
    data.push_back(face.c.normal.z);
  }

  GLuint newId = 0;
  glGenBuffers(1, &newId);
  buffer = newId;
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data.at(0)) * data.size(), &data.at(0), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::addFace(Vector3 a, Vector3 b, Vector3 c)
{
  Face f;
  f.a.position = a;
  f.b.position = b;
  f.c.position = c;
  faces.push_back(f);
}

void Mesh::addFace(Vertex a, Vertex b, Vertex c)
{
  Face f;
  f.a = a;
  f.b = b;
  f.c = c;
  faces.push_back(f);
}

Mesh::~Mesh()
{
  if(!buffer) return;
  GLuint oldId = 0;
  oldId = buffer;
  glDeleteBuffers(1, &oldId);
}

void World::processUsemtlLine(std::vector<std::string>& splitLine, DataStore& store)
{
  std::string texPath;

  for(size_t i = 0; i < store.materialInfos.size(); i++)
  {
    if(store.materialInfos.at(i).name == splitLine.at(1))
    {
      texPath = store.materialInfos.at(i).texturePath;
      break;
    }
  }

  store.currentMg.reset();

  //std::cout << "Path: " << texPath << std::endl;

  for(size_t i = 0; i < materialGroups.size(); i++)
  {
    if(materialGroups.at(i)->texture &&
      materialGroups.at(i)->texture->getPath() == texPath)
    {
      store.currentMg = materialGroups.at(i);
      break;
    }

    if(!materialGroups.at(i)->texture && texPath == "")
    {
      store.currentMg = materialGroups.at(i);
      break;
    }
  }

  if(!store.currentMg.get())
  {
    //std::cout << "Adding new material group for [" << texPath << "]" << std::endl;
    std::shared_ptr<MaterialGroup> mg = std::make_shared<MaterialGroup>();

    try
    {
      mg->texture = Texture::load(texPath);
    }
    catch(std::exception& e)
    {
      std::cout << "Warning: Buggy model: " << path << " [" << texPath << "]" << std::endl;
    }

    store.currentMg = mg;
    materialGroups.push_back(mg);
  }
}

void World::processMtlLibLine(std::vector<std::string>& splitLine, DataStore& store)
{
  size_t lastDir = 0;
  std::string folder = path;

  for(size_t i = 0; i < path.length(); i++)
  {
    if(path.at(i) == '/' || path.at(i) == '\\')
    {
      lastDir = i;
    }
  }

  if(lastDir != 0)
  {
    folder = folder.substr(0, lastDir);
  }

  std::string mtlPath = folder + "/" + splitLine.at(1);
  //std::cout << "MtlLib: " << mtlPath << std::endl;

  std::ifstream file(std::string(Environment::getAssetsDirectory() + "/" + mtlPath).c_str());

  if(!file.is_open())
  {
    throw std::exception();
  }

  std::vector<std::string> splitLineM;

  while(!file.eof())
  {
    std::string line;
    getline(file, line);
    Util::splitStringWhitespace(line, splitLineM);

    if(splitLineM.size() < 1) continue;

    if(splitLineM.at(0) == "newmtl")
    {
      //std::cout << "New Mtl: " << splitLineM.at(1) << std::endl;
      MaterialInfo mi;
      mi.name = splitLineM.at(1);
      store.materialInfos.push_back(mi);
    }
    else if(splitLineM.at(0) == "map_Kd")
    {
      std::string texPath = splitLineM.at(1);
      lastDir = 0;

      for(size_t i = 0; i < texPath.length(); i++)
      {
        if(texPath.at(i) == '.')
        {
          lastDir = i;
        }
      }

      if(lastDir != 0)
      {
        texPath = texPath.substr(0, lastDir);
      }

      //std::cout << "Texture: " << folder << "/" << texPath << std::endl;
      store.materialInfos.at(store.materialInfos.size() - 1).texturePath = folder + "/" + texPath;
    }
  }
}

void World::processVLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.positions.push_back(Vector3(
    atof(splitLine.at(1).c_str()),
    atof(splitLine.at(2).c_str()),
    atof(splitLine.at(3).c_str())
  ));
}

void World::processVtLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.texCoords.push_back(Vector2(
    atof(splitLine.at(1).c_str()),
    1.0f - atof(splitLine.at(2).c_str())
  ));
}

void World::processVnLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.normals.push_back(Vector3(
    atof(splitLine.at(1).c_str()),
    atof(splitLine.at(2).c_str()),
    atof(splitLine.at(3).c_str())
  ));
}

void World::processOLine(std::vector<std::string>& splitLine, DataStore& store)
{
  if(splitLine.at(1).length() > 3 && splitLine.at(1).substr(0, 2) == "e_")
  {
    store.currentEntity = std::make_shared<EntitySpawnInfo>();
    entitySpawnInfos.push_back(store.currentEntity);
    store.currentEntity->type = splitLine.at(1).substr(2);
    std::cout << "Entity: " << store.currentEntity->type << std::endl;
  }
  else
  {
    store.currentEntity.reset();
  }
}

Face Mesh::getFace(size_t i)
{
  return faces.at(i);
}

void World::processFLine(std::vector<std::string>& splitLine, DataStore& store)
{
  if(store.currentEntity.get() != NULL)
  {
    EntitySpawnInfo* esi = store.currentEntity.get();
    //std::cout << "Processing entity: " << esi->type << std::endl;

    for(size_t i = 1; i < splitLine.size(); i++)
    {
      Util::splitString(splitLine.at(i), '/', store.tokens);
      Vector3 position;

      position = store.positions.at(atoi(store.tokens.at(0).c_str()) - 1);

      if(!esi->positionSet)
      {
        esi->position = position;
        esi->positionSet = true;
      }
      else
      {
        esi->position = (esi->position + position) / 2.0f;
      }
    }

    // Comment out to keep mesh
    return;
  }

  if(!store.currentMg)
  {
    std::cout << "No current material defined" << std::endl;
    throw std::exception();
    //std::shared_ptr<MaterialGroup> mg = std::make_shared<MaterialGroup>();
    //store.currentMg = mg;
    //materialGroups.push_back(mg);
  }

  Mesh* mesh = store.currentMg->mesh.get();
  std::vector<Vertex> vertices;

  for(size_t i = 1; i < splitLine.size(); i++)
  {
    Util::splitString(splitLine.at(i), '/', store.tokens);
    Vertex vertex;

    vertex.position = store.positions.at(atoi(store.tokens.at(0).c_str()) - 1);
    vertex.texCoord = store.texCoords.at(atoi(store.tokens.at(1).c_str()) - 1);
    vertex.normal = store.normals.at(atoi(store.tokens.at(2).c_str()) - 1);

    vertices.push_back(vertex);
  }

  mesh->addFace(vertices.at(0), vertices.at(1), vertices.at(2));

  if(vertices.size() > 3)
  {
    mesh->addFace(vertices.at(2), vertices.at(3), vertices.at(0));
  }
}

void World::generateVbos()
{
  for(size_t i = 0; i < materialGroups.size(); i++)
  {
    Mesh* mesh = materialGroups.at(i)->mesh.get();

    if(mesh->faces.size() < 1)
    {
      materialGroups.erase(materialGroups.begin() + i);
      i--;
      continue;
    }

    mesh->generateVbos();
  }
}

void World::onInitialize(std::string path)
{
  Entity* ent = getEntity();
  ent->addTag("world");

  this->path = path;
  path += ".obj";
  std::ifstream file(std::string(Environment::getAssetsDirectory() + "/" + path).c_str());

  if(!file.is_open())
  {
    std::cerr << "World File at \"" << path << "\" failed to open\n";
    throw std::exception();
  }

  DataStore store;
  std::string line;
  std::vector<std::string> splitLine;

  while(!file.eof())
  {
    getline(file, line);
    Util::splitStringWhitespace(line, splitLine);

    if(splitLine.size() < 1) continue;

    if(splitLine.at(0) == "v")
    {
      processVLine(splitLine, store);
    }
    else if(splitLine.at(0) == "vt")
    {
      processVtLine(splitLine, store);
    }
    else if(splitLine.at(0) == "vn")
    {
      processVnLine(splitLine, store);
    }
    else if(splitLine.at(0) == "f")
    {
      processFLine(splitLine, store);
    }
    else if(splitLine.at(0) == "o" || splitLine.at(0) == "g")
    {
      processOLine(splitLine, store);
    }
    else if(splitLine.at(0) == "usemtl")
    {
      processUsemtlLine(splitLine, store);
    }
    else if(splitLine.at(0) == "mtllib")
    {
      processMtlLibLine(splitLine, store);
    }
  }

  generateVbos();

  WorldRenderer* wr = ent->addComponent<WorldRenderer>();
  WorldCollider* wc = ent->addComponent<WorldCollider>();
}

}
