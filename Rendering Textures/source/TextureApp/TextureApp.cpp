#include "TextureApp.h"
#include <STB/stb_image.h>
#include <GLCORE/gl_core_4_4.h>

void TextureApp::AfterInit() {
    int imageWidth = 0, imageHeight = 0, imageFormat = 0;
    unsigned char* imageData = stbi_load( "./textures/crate.png", &imageWidth, &imageHeight, &imageFormat, STBI_default );

    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    stbi_image_free( imageData );
}

TextureApp::TextureApp() :
    TextureApp( "TextureApp" ) {}

TextureApp::TextureApp( const std::string set_name ) :
    TextureApp( set_name, 1280, 720 ) {}

TextureApp::TextureApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ), textureID( 0 ) {}

TextureApp::~TextureApp() {}
