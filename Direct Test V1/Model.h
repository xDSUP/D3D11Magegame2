#pragma once
#include "D3D11_Framework.h"


#define NOMINMAX
#undef min
#undef max
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AsimpMesh.h"


using namespace D3D11Framework;
using namespace std;

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	HEIGHT,
	AMBIENT
};

struct Texture
{
    unsigned int id;
    TextureType type;
    string path;
    ID3D11ShaderResourceView* texture;
};


class Model
{
public:
    Model(Render* render)
    {
        this->render = render;
    }

	bool Init(const char* path)
    {
        loadModel(path);
        return true;
    }
    void Draw(CXMMATRIX viewMatrix);

    void Translate(float x, float y, float z);
    void Identity();
    void Rotate(float angle, float x, float y, float z);
    void Scale(float x, float y, float z);
private:

    // Данные модели
    Render* render;
    vector<AsimpMesh> meshes;
    string directory;
    vector<Texture> textures_loaded;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    AsimpMesh* processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);

};

