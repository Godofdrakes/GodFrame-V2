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
    bool useWireframe;
    Object( const char* fbx_file_path );
    ~Object();

    void Render( const unsigned int program_id,
                 const glm::mat4& projection_view,
                 const glm::vec3& camera_position );
};

#endif
