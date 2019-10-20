#include "hydra.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace hydra
{

void Model::processUsemtlLine(std::vector<std::string>& splitLine,
  DataStore& store)
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

/*
  if(texPath.length() < 1)
  {
    //std::cout << "Material specified does not exist" << std::endl;
    //throw std::exception();
    texPath = "internal/textures/null";
  }
*/

  //std::cout << "Using: " << texPath << std::endl;
  store.currentMg.reset();

  for(size_t i = 0; i < store.currentPart->materialGroups.size(); i++)
  {
    if(store.currentPart->materialGroups.at(i)->texture->getPath() == texPath)
    {
      store.currentMg = store.currentPart->materialGroups.at(i);
      break;
    }
  }

  if(!store.currentMg.get())
  {
    //std::cout << "Adding new material group" << std::endl;
    std::shared_ptr<MaterialGroup> mg = std::make_shared<MaterialGroup>();

    if(texPath == "")
    {
      // TODO: Leak
      //mg->texture = new Texture(Vector4(1, 0, 0, 1));
    }
    else
    {
      mg->texture = Texture::load(texPath);
      store.currentPart->materialGroups.push_back(mg);
    }

    store.currentMg = mg;
  }
}

void Model::processMtlLibLine(std::vector<std::string>& splitLine,
  DataStore& store)
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

  std::string mtlPath = Environment::getAssetsDirectory() + "/" + folder + "/" + splitLine.at(1);
  //std::cout << "MtlLib: " << mtlPath << std::endl;

  std::ifstream file(mtlPath.c_str());

  if(!file.is_open())
  {
    throw std::exception();
  }

  std::string line;
  std::vector<std::string> splitLineM;

  while(!file.eof())
  {
    getline(file, line);
    //splitLineM.clear();
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

void Model::processVLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.positions.push_back(Vector3(
    atof(splitLine.at(1).c_str()),
    atof(splitLine.at(2).c_str()),
    atof(splitLine.at(3).c_str())
  ));
}

void Model::processVtLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.texCoords.push_back(Vector2(
    atof(splitLine.at(1).c_str()),
    atof(splitLine.at(2).c_str())
  ));
}

void Model::processVnLine(std::vector<std::string>& splitLine, DataStore& store)
{
  store.normals.push_back(Vector3(
    atof(splitLine.at(1).c_str()),
    atof(splitLine.at(2).c_str()),
    atof(splitLine.at(3).c_str())
  ));
}

void Model::processOLine(std::vector<std::string>& splitLine, DataStore& store)
{
  std::shared_ptr<Part> part = std::make_shared<Part>();
  parts.push_back(part);
  store.currentPart = part.get();
  part->name = splitLine.at(1);
  //std::cout << "Part: " << splitLine.at(1) << std::endl;
}

void Model::processFLine(std::vector<std::string>& splitLine, DataStore& store)
{
  if(!store.currentMg)
  {
    std::cout << "No current material defined" << std::endl;
    throw std::exception();
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

Model* Model::load(std::string path)
{
  for(size_t i = 0; i < Environment::instance->models.size(); i++)
  {
    if(Environment::instance->models.at(i)->path == path)
    {
      return Environment::instance->models.at(i).get();
    }
  }

  std::shared_ptr<Model> rtn = std::make_shared<Model>();
  Environment::instance->models.push_back(rtn);

  rtn->path = path;
  path += ".obj";
  std::ifstream file(std::string(Environment::getAssetsDirectory() + "/" + path).c_str());

  if(!file.is_open())
  {
    throw std::exception();
  }

  DataStore store;
  std::string line;
  std::vector<std::string> splitLine;

  while(!file.eof())
  {
    //splitLine.clear();
    getline(file, line);
    Util::splitStringWhitespace(line, splitLine);

    if(splitLine.size() < 1) continue;

    if(splitLine.at(0) == "v")
    {
      rtn->processVLine(splitLine, store);
    }
    else if(splitLine.at(0) == "vt")
    {
      rtn->processVtLine(splitLine, store);
    }
    else if(splitLine.at(0) == "vn")
    {
      rtn->processVnLine(splitLine, store);
    }
    else if(splitLine.at(0) == "f")
    {
      rtn->processFLine(splitLine, store);
    }
    else if(splitLine.at(0) == "o" || splitLine.at(0) == "g")
    {
      rtn->processOLine(splitLine, store);
    }
    else if(splitLine.at(0) == "usemtl")
    {
      rtn->processUsemtlLine(splitLine, store);
    }
    else if(splitLine.at(0) == "mtllib")
    {
      rtn->processMtlLibLine(splitLine, store);
    }
  }

  rtn->generateOffsets();
  rtn->generateVbos();

  return rtn.get();
}

void Model::generateOffsets()
{
  Vector3 totalMax(
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min());

  Vector3 totalMin(
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max());

  for(size_t pi = 0; pi < parts.size(); pi++)
  {
    Vector3 max(
      std::numeric_limits<float>::min(),
      std::numeric_limits<float>::min(),
      std::numeric_limits<float>::min());

    Vector3 min(
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::max());

    for(size_t mgi = 0; mgi < parts.at(pi)->materialGroups.size(); mgi++)
    {
      Mesh* mesh = parts.at(pi)->materialGroups.at(mgi)->mesh.get();

      for(size_t fi = 0; fi < mesh->faces.size(); fi++)
      {
        Face& f = mesh->faces.at(fi);
        if(f.a.position.x > max.x) max.x = f.a.position.x;
        if(f.a.position.y > max.y) max.y = f.a.position.y;
        if(f.a.position.z > max.z) max.z = f.a.position.z;
        if(f.b.position.x > max.x) max.x = f.b.position.x;
        if(f.b.position.y > max.y) max.y = f.b.position.y;
        if(f.b.position.z > max.z) max.z = f.b.position.z;
        if(f.c.position.x > max.x) max.x = f.c.position.x;
        if(f.c.position.y > max.y) max.y = f.c.position.y;
        if(f.c.position.z > max.z) max.z = f.c.position.z;

        if(f.a.position.x < min.x) min.x = f.a.position.x;
        if(f.a.position.y < min.y) min.y = f.a.position.y;
        if(f.a.position.z < min.z) min.z = f.a.position.z;
        if(f.b.position.x < min.x) min.x = f.b.position.x;
        if(f.b.position.y < min.y) min.y = f.b.position.y;
        if(f.b.position.z < min.z) min.z = f.b.position.z;
        if(f.c.position.x < min.x) min.x = f.c.position.x;
        if(f.c.position.y < min.y) min.y = f.c.position.y;
        if(f.c.position.z < min.z) min.z = f.c.position.z;
      }
    }

    parts.at(pi)->size = max - min;

    parts.at(pi)->offset = (min + max) / 2.0f;
    //parts.at(pi)->offset *= 1.5f;

/*
    std::cout << "Center: " <<
      parts.at(pi)->name << " " <<
      parts.at(pi)->offset.x << " " <<
      parts.at(pi)->offset.y << " " <<
      parts.at(pi)->offset.z << std::endl;

    std::cout << "Size: " <<
      parts.at(pi)->name << " " <<
      parts.at(pi)->size.x << " " <<
      parts.at(pi)->size.y << " " <<
      parts.at(pi)->size.z << std::endl;

    std::cout << "Size: " <<
      parts.at(pi)->name << " " <<
      max.y << " " <<
      min.y << " " << std::endl;
*/

    for(size_t mgi = 0; mgi < parts.at(pi)->materialGroups.size(); mgi++)
    {
      Mesh* mesh = parts.at(pi)->materialGroups.at(mgi)->mesh.get();

      for(size_t fi = 0; fi < mesh->faces.size(); fi++)
      {
        Face& f = mesh->faces.at(fi);
        f.a.position -= parts.at(pi)->offset;
        f.b.position -= parts.at(pi)->offset;
        f.c.position -= parts.at(pi)->offset;
      }
    }

    if(max.x > totalMax.x) totalMax.x = max.x;
    if(max.y > totalMax.y) totalMax.y = max.y;
    if(max.z > totalMax.z) totalMax.z = max.z;

    if(min.x < totalMin.x) totalMin.x = min.x;
    if(min.y < totalMin.y) totalMin.y = min.y;
    if(min.z < totalMin.z) totalMin.z = min.z;
  }

  size = totalMax - totalMin;
  //size = Vector3(100, 100, 10);
  center = (totalMin + totalMax) / 2.0f;
}

void Model::generateVbos()
{
  for(size_t pi = 0; pi < parts.size(); pi++)
  {
    for(size_t mgi = 0; mgi < parts.at(pi)->materialGroups.size(); mgi++)
    {
      Mesh* mesh = parts.at(pi)->materialGroups.at(mgi)->mesh.get();

      if(mesh->faces.size() < 1)
      {
        parts.at(pi)->materialGroups.erase(
          parts.at(pi)->materialGroups.begin() + mgi);

        mgi--;
        continue;
      }

/*
      std::vector<GLfloat> positions;
      std::vector<GLfloat> normals;
      std::vector<GLfloat> texCoords;

      for(size_t f = 0; f < mesh->faces.size(); f++)
      {
        Face face = mesh->faces.at(f);
        positions.push_back(face.a.position.x);
        positions.push_back(face.a.position.y);
        positions.push_back(face.a.position.z);
        positions.push_back(face.b.position.x);
        positions.push_back(face.b.position.y);
        positions.push_back(face.b.position.z);
        positions.push_back(face.c.position.x);
        positions.push_back(face.c.position.y);
        positions.push_back(face.c.position.z);

        normals.push_back(face.a.normal.x);
        normals.push_back(face.a.normal.y);
        normals.push_back(face.a.normal.z);
        normals.push_back(face.b.normal.x);
        normals.push_back(face.b.normal.y);
        normals.push_back(face.b.normal.z);
        normals.push_back(face.c.normal.x);
        normals.push_back(face.c.normal.y);
        normals.push_back(face.c.normal.z);

        texCoords.push_back(face.a.texCoord.x);
        texCoords.push_back(face.a.texCoord.y);
        texCoords.push_back(face.b.texCoord.x);
        texCoords.push_back(face.b.texCoord.y);
        texCoords.push_back(face.c.texCoord.x);
        texCoords.push_back(face.c.texCoord.y);
      }

      glGenBuffers(1, &mesh->positionBuffer.data());
      //printf("Buffer: %i\n", mesh->positionBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->positionBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(positions.at(0)) * positions.size(), &positions.at(0), GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenBuffers(1, &mesh->normalBuffer.data());
      //printf("Buffer: %i\n", mesh->normalBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(normals.at(0)) * normals.size(), &normals.at(0), GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenBuffers(1, &mesh->texCoordBuffer.data());
      //printf("Buffer: %i\n", mesh->texCoordBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->texCoordBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords.at(0)) * texCoords.size(), &texCoords.at(0), GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
*/

      mesh->generateVbos();
    }
  }
}

Vector3 Model::getSize()
{
  return size;
}

Vector3 Model::getCenter()
{
  return center;
}

}
