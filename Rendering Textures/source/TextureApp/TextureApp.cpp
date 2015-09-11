#include "TextureApp.h"

TextureApp::TextureApp() : TextureApp( "TextureApp" ) {}

TextureApp::TextureApp( const std::string set_name ) : TextureApp( set_name, 1280, 720 ) {}

TextureApp::TextureApp( const std::string set_name, const int set_width, const int set_height ) : Application( set_name, set_width, set_height ) {}

TextureApp::~TextureApp() {}
