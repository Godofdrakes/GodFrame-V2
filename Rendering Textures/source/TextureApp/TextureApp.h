#pragma once
#ifndef _TextureApp_H_
#define _TextureApp_H_

#include <Application.h>
class Camera;

class TextureApp : public Application {
    unsigned int vao, vbo, ibo, programID, textureID;
    Camera* camera;
protected:
    bool BuildShaders(const char* pathToVertexShader, const char* pathToFragmentShader);
    unsigned int LoadShader(const char* pathToShader, unsigned int shaderType);
    virtual void Init_Logic() override;
    virtual void FixedUpdate_Logic() override;
    virtual void Render_Logic() override;
public:
    TextureApp();
    TextureApp( const std::string set_name );
    TextureApp( const std::string set_name, const int set_width, const int set_height );
    ~TextureApp();
};

#endif
