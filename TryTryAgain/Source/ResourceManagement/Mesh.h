#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace rm
{
    class Shader;
    class Texture2D;
    class Material;

    using namespace std;

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };


	class Mesh
	{
	public:
        // mesh Data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        std::shared_ptr<rm::Material> material;
        unsigned int VAO;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<std::shared_ptr<rm::Texture2D>> textures, const char* name);
        Mesh() {};
        ~Mesh();

		void Draw(rm::Shader* shader);

        std::string GetName();

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();

        std::string name;

	};
}


