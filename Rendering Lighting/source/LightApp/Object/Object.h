#pragma once
#ifndef _Object_H_
#define _Object_H_
#include <glm/glm.hpp>

class FBXFile;

class Object {
    unsigned int vao, vbo, ibo;
    unsigned int* textureID;
    unsigned int textureCount, indexCount;
public:
    bool uHasColor, uHasNormal, uHasTangent, uHasBiNormal, uHasIndices, uHasWeights, useWireframe;
    Object( const char* fbx_file_path );
    ~Object();

    void Render( const unsigned int program_id,
                 const glm::mat4& projection_view );
};

#endif
