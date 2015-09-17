#pragma once
#ifndef _FbxApp_H_
#define _FbxApp_H_

#include <Application.h>
#include <FBX/FBXFile.h>

class Camera;

class FbxApp : public Application {
    unsigned int vao, vbo, ibo, programID, textureID;
    Camera* camera;
    FBXFile fbxFile;
protected:
    void InitCamera();
    bool BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader );
    unsigned int LoadShader( const char* pathToShader, unsigned int shaderType );
    virtual void Init_Logic() override;
    virtual void FixedUpdate_Logic() override;
    virtual void Render_Logic() override;
    virtual void Shutdown_Logic() override;
public:
    FbxApp();
    FbxApp( const std::string set_name );
    FbxApp( const std::string set_name, const int set_width, const int set_height );
    ~FbxApp();
};

#endif
