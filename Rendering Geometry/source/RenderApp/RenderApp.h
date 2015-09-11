#pragma once
#ifndef _RENDERAPP_H_
#define _RENDERAPP_H_

#include <Application.h>

class Camera;

class RenderApp : public Application {
    void GenerateGrid( unsigned int rows, unsigned int cols );
    unsigned int vao, vbo, ibo, programID;

    virtual void AfterInit() override;
    virtual void BeforeUpdate() override;
    virtual void BeforeRender() override;
    virtual void BeforeShutdown() override;

    Camera* camera;

    // Returns true if it worked.
    bool BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader );

    // Returns handle to shader, or 0 if it failed.
    unsigned int LoadShader( const char* pathToShader, unsigned int shaderType );

public:
    RenderApp();
    RenderApp( const std::string set_name );
    RenderApp( const std::string set_name, const int set_width, const int set_height );
    ~RenderApp();
};

#endif
