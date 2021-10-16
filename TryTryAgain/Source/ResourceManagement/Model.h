#pragma once
#include <vector>
#include <string>
#include <memory>

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

    class mNode
    {
    public:
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::unique_ptr<mNode>> children;
    };

    class Model : public rm::Resource
    {
    public:

        // All of the meshes in the model file, no hierarchy structure
        vector<std::shared_ptr<Mesh>>    meshes;

        // our hierarchy structure
        std::unique_ptr<mNode> HierarchyRoot;

        std::string directory;
        std::string Path;
        bool gammaCorrection = false;

        // constructor, expects a filepath to a 3D model.
        Model() {}
        ~Model() override;

        void Init(std::string path) override;

        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const& path);

        std::string GetName();

    private:

        std::string name;

        // process the entire aiScene
        void processScene(const aiScene* scene);
        
        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* aiNode, const aiScene* scene, mNode* mNode);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<std::shared_ptr<rm::Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}