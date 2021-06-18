#pragma once

#include "Model.h"

#include <iostream>
#include "stdafx.h"
#include "windows.h"

#include "Util.h"


void Model::Draw(CXMMATRIX viewMatrix)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(viewMatrix);
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Обрабатываем все меши (если они есть) у выбранного узла
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(*processMesh(mesh, scene));
    }
    // И проделываем то же самое для всех дочерних узлов
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

AsimpMesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<unsigned short> indices;
    auto resultMesh = new AsimpMesh(render);
    resultMesh->Init();
	
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // Координаты
        vertex.Pos.x = mesh->mVertices[i].x;
        vertex.Pos.y = mesh->mVertices[i].y;
        vertex.Pos.z = mesh->mVertices[i].z;

        // Нормали
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;

        // Текстурные координаты
        if (mesh->mTextureCoords[0]) // если меш содержит текстурные координаты
        {
            // Вершина может содержать до 8 различных текстурных координат. Мы предполагаем, что мы не будем использовать модели,
            // в которых вершина может содержать несколько текстурных координат, поэтому мы всегда берем первый набор (0)
            vertex.Tex.x = mesh->mTextureCoords[0][i].x;
            vertex.Tex.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.Tex.x = 0.0f;
            vertex.Tex.y = 0.0f;
        }

        // Касательный вектор
        vertex.Tangent.x = mesh->mTangents[i].x;
        vertex.Tangent.y = mesh->mTangents[i].y;
        vertex.Tangent.z = mesh->mTangents[i].z;

        //// Вектор бинормали
        //vector.x = mesh->mBitangents[i].x;
        //vector.y = mesh->mBitangents[i].y;
        //vector.z = mesh->mBitangents[i].z;
        //vertex.Bitangent = vector;
        
        // Обрабатываем координаты вершин, нормали и текстурные координаты
        vertices.push_back(vertex);
    }
    // Теперь проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Получаем все индексы граней и сохраняем их в векторе indices
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

	
    resultMesh->InitBuffers(vertices.size(), indices.size(), &indices[0], &vertices[0]);

    // проверка
    Vertex* arr = &vertices[0];
    for (unsigned int j = 0; j < vertices.size(); j++)
        if (arr[j].Pos.x != vertices[j].Pos.x ||
            arr[j].Pos.y != vertices[j].Pos.y ||
            arr[j].Pos.z != vertices[j].Pos.z)
            throw - 213;
    sLog->Debug("Загружен mesh с %d вершин,и %d индексов", vertices.size(), indices.size());
    // Обрабатываем материалы
    
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Material mat;
	material->Get(AI_MATKEY_COLOR_AMBIENT, mat.ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, mat.diffuse);
	material->Get(AI_MATKEY_COLOR_REFLECTIVE, mat.reflect);
	material->Get(AI_MATKEY_COLOR_SPECULAR, mat.specular);
    resultMesh->SetMaterial(mat);
    // Мы вводим соглашение об именах сэмплеров в шейдерах. Каждая диффузная текстура будет называться 'texture_diffuseN',
    // где N - это порядковый номер от 1 до MAX_SAMPLER_NUMBER. 
    // То же самое относится и к другим текстурам:
    // диффузная - texture_diffuseN
    // отражения - texture_specularN
    // нормали - texture_normalN

    // 1. Диффузные карты
    vector<Texture> textures;
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    //// 2. Карты отражения
    //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    // 3. Карты нормалей
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::HEIGHT);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    
    // 4. Карты высот
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::AMBIENT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    for (int i = 0; i < textures.size(); ++i)
    {
        resultMesh->GetShader()->AddTexture(textures[i].texture);
    }
	
    // Возвращаем mesh-объект, созданный на основе полученных данных
    vertices.clear();
    indices.clear();
    return resultMesh;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Проверяем, не была ли текстура загружена ранее, и если - да, то пропускаем загрузку новой текстуры и переходим к следующей итерации
        bool skip = false;
        for (unsigned int j = 0; j < this->textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // текстура с тем же путем к файлу уже загружена, переходим к следующей (оптимизация)
                break;
            }
        }
        if (!skip)
        {   // Если текстура еще не была загружена - загружаем её
            Texture texture;
            
            HRESULT hr = D3DX11CreateShaderResourceViewFromFile(render->GetDevice(), w(str.C_Str()), NULL, NULL, &texture.texture, NULL);
            if (FAILED(hr))
            {
                Log::Get()->Err("Не удалось загрузить текстуру %ls", str.C_Str());
            }
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // сохраняем её в массиве с уже загруженными текстурами, тем самым гарантируя, что у нас не появятся дубликаты текстур
        }
    }
    return textures;
}

void Model::Translate(float x, float y, float z)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Translate(x, y, z);
}

void Model::Identity()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Identity();
}

void Model::Rotate(float angle, float x, float y, float z)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Rotate(angle, x, y, z);
}

void Model::Scale(float x, float y, float z)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Scale(x, y, z);
}
