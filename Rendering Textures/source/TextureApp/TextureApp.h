#pragma once
#ifndef _TextureApp_H_
#define _TextureApp_H_

#include <Application.h>

class TextureApp : public Application {
    unsigned int texture;
protected:
    virtual void AfterInit() override;
public:
    TextureApp();
    TextureApp( const std::string set_name );
    TextureApp( const std::string set_name, const int set_width, const int set_height );
    ~TextureApp();
};

#endif
