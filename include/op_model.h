#pragma once

#include "shader.h"
#include "op_mesh.h"

#include <vector>

class Model
{
    public:
        Model(char* path);
        void Draw(Shader shader);

    private:
        std::vector<OpMesh> meshes;
};