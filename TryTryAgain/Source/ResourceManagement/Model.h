#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Resource.h"

using namespace std;

namespace rm
{

    class Model : public rm::Resource
    {
    public:
        // model data 
        vector<Mesh>    meshes;
        std::string directory;
        std::string Path;
        bool gammaCorrection = false;

        // constructor, expects a filepath to a 3D model.
        Model() {}
        ~Model() override;

        void Init(std::string path) override;

        // draws the model, and thus all its meshes
        void Draw(Shader* shader);

        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const& path);

    private:
        
        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<std::shared_ptr<rm::Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}