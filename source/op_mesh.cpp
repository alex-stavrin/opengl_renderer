#include "op_mesh.h"

OpMesh::OpMesh(const std::vector<Vertex>& n_vertices, const std::vector<unsigned> n_indices,
    const std::vector<Texture>& n_textures)
{
    vertices = n_vertices;
    indices = n_indices;
    textures = n_textures;

    SetupMesh();
}

void OpMesh::Draw(const Shader& shader)
{

}

void OpMesh::SetupMesh()
{
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &element_buffer_object);

    /////
}