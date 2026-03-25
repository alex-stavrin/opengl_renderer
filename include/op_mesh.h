#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texture_coordinates;
};

struct Texture
{
    unsigned id;
    std::string type;  
};

class OpMesh
{
    public:
    
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        std::vector<Texture> textures;

        OpMesh(const std::vector<Vertex>& n_vertices, const std::vector<unsigned> n_indices,
            const std::vector<Texture>& n_textures);

        void Draw(const Shader& shader);

    private:

        unsigned vertex_array_object;
        unsigned vertex_buffer_object;
        unsigned element_buffer_object;

        void SetupMesh();
};