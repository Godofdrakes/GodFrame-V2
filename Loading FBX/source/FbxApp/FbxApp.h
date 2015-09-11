#pragma once
#ifndef _FbxApp_H_
#define _FbxApp_H_

#include <Application.h>
class Camera;

class FbxApp : public Application {
    unsigned int vao, vbo, ibo, programID, textureID;
    Camera* camera;
protected:
    bool BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader );
    unsigned int LoadShader( const char* pathToShader, unsigned int shaderType );
    virtual void AfterInit() override;
    virtual void BeforeUpdate() override;
    virtual void BeforeRender() override;
    virtual void AfterRender() override;
public:
    FbxApp();
    FbxApp( const std::string set_name );
    FbxApp( const std::string set_name, const int set_width, const int set_height );
    ~FbxApp();
};

#endif
