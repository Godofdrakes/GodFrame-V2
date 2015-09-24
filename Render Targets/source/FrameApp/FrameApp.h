#pragma once
#ifndef _FrameApp_H_
#define _FrameApp_H_

#include <Application.h>

class Camera;

class FrameApp : public Application {
    unsigned int programID;
    Camera* camera;
    unsigned int frameBuffer, frameTexture, frameDepth,
                 mirrorVAO, mirrorVBO, mirrorIBO;
protected:
    void InitCamera();
    bool BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader );
    unsigned int LoadShader( const char* pathToShader, unsigned int shaderType );
    virtual void Init_Logic() override;
    virtual void FixedUpdate_Logic() override;
    virtual void Render_Logic() override;
    virtual void Shutdown_Logic() override;
public:
    FrameApp();
    FrameApp( const std::string set_name );
    FrameApp( const std::string set_name, const int set_width, const int set_height );
    ~FrameApp();
};

#endif
