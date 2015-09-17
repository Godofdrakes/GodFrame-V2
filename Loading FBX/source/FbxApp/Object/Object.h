#pragma once
#ifndef _Object_H_
#define _Object_H_
#include <glm/mat4x2.hpp>

class FBXFile;

class Object {
    unsigned int vao, vbo, ibo;
    unsigned int* textureID;
    FBXFile* file;
    unsigned int textureCount;
public:
    Object( const char* fbx_file_path );
    ~Object();

    void Render( const unsigned int program_id,
                 const glm::mat4& projection_view,
                 const unsigned int diffuse = 0 );
};

#endif
