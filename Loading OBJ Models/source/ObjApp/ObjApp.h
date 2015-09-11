#pragma once
#ifndef _OBJAPP_H_
#define _OBJAPP_H_

#include <Application.h>
#include <TINYOBJ/tiny_obj_loader.h>
#include "OpenGLInfo.h"

class Camera;

class ObjApp : public Application {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<OpenGLInfo> glInfo;
    unsigned int vao, vbo, ibo, programID;

    Camera* camera;

    // Returns true if it worked.
    bool BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader );

    // Returns handle to shader, or 0 if it failed.
    unsigned int LoadShader( const char* pathToShader, unsigned int shaderType );

    void CreateOpenGLBuffers( std::vector<tinyobj::shape_t>& shapes );
    void DrawOpenGLBuffers( std::vector<tinyobj::shape_t>& shapes );

    virtual void AfterInit() override;
    virtual void BeforeUpdate() override;
    virtual void BeforeRender() override;
    virtual void BeforeShutdown() override;
public:
    ObjApp();
    ObjApp( const std::string set_name );
    ObjApp( const std::string set_name, const int set_width, const int set_height );
    ~ObjApp();

};

#endif
