#include "Model.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <Panels/Console.h>
#include <ResourceManagement/ResourceManager.h>
#include "Material.h"

namespace rm
{
    Model::~Model()
    {
        // delete model data
    }
    void Model::Init(std::string path)
    {
        gammaCorrection = false;
        HierarchyRoot = std::make_unique<mNode>();
        loadModel(path);
    }

    void rm::Model::loadModel(string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            M_LOG("[error]::ASSIMP:: %s", importer.GetErrorString());
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));
        this->Path = path;

        this->name = scene->GetShortFilename(path.c_str());
        auto i = name.find('.');
        if (i != std::string::npos)
            name = name.erase(i);
        processScene(scene);
    }

    std::string Model::GetName()
    {
        return name;
    }

    void Model::processScene(const aiScene* scene)
    {
        // process all unique meshes in scene
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            this->meshes.push_back(std::make_shared<rm::Mesh>(processMesh(scene->mMeshes[i], scene)));
        }

        // get the hierarchy structure from assimp so that we can use it in our hierarchy
        processNode(scene->mRootNode, scene, HierarchyRoot.get());
    }

    void rm::Model::processNode(aiNode* aiNode, const aiScene* scene, mNode* mnode)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            // we store this in our mNode (intermediate hierarchy stucture)
            mnode->meshes.push_back(this->meshes[aiNode->mMeshes[i]]);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
        {
            // add this child to the mNode (Intermediate hierarchy)
            mnode->children.push_back(std::make_unique<mNode>());

            // we can now process the next child, the data will be stored in the correct child mNode
            processNode(aiNode->mChildren[i], scene, mnode->children[i].get());
        }
    }

    rm::Mesh rm::Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<std::shared_ptr<rm::Texture2D>> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<std::shared_ptr<rm::Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<std::shared_ptr<rm::Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        vector<std::shared_ptr<rm::Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        vector<std::shared_ptr<rm::Texture2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures, mesh->mName.C_Str());
    }

    vector<std::shared_ptr<rm::Texture2D>> rm::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<std::shared_ptr<rm::Texture2D>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture

            string filename = string(str.C_Str());
            filename = directory + '/' + filename;

            auto sp = rm::ResourceManager::Load<rm::Texture2D>(filename);
            sp->type = typeName;
            sp->path = str.C_Str();
            textures.push_back(sp);
                
        }
        return textures;
    }
}


