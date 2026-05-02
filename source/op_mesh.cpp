#include <cstddef>

#include "error_printer.h"
#include "op_mesh.h"

OpMesh::OpMesh(const std::vector<Vertex>& n_vertices, const std::vector<unsigned> n_indices,
    const std::vector<Texture>& n_textures)
{
    vertices = n_vertices;
    indices = n_indices;
    textures = n_textures;

    SetupMesh();
}

void OpMesh::SetupMesh()
{
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &element_buffer_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));

    glBindVertexArray(0);
}

void OpMesh::Draw(const Shader& shader)
{
    unsigned diffuse_number = 1;
    unsigned specular_number = 1;
    unsigned normal_number = 1;
    unsigned height_number = 1;
    for (unsigned i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string texture_type = textures[i].type;
        std::string number;
        if (texture_type == "texture_diffuse")
        {
            number = std::to_string(diffuse_number);
            diffuse_number++;
        }
        else if (texture_type == "texture_specular")
        {
            number = std::to_string(specular_number);
            specular_number++;
        }
        else if (texture_type == "texture_normal")
        {

        }
        else if (texture_type == "texture_height")
        {

        }
        else
        {
            ErrorPrinter::PrintError("Unknown type of texture");
        }

        GLint texture_location = glGetUniformLocation(shader.id, (texture_type + number).c_str());
        glUniform1i(texture_location, i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(vertex_array_object);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}