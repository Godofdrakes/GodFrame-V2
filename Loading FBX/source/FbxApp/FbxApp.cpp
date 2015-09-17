#include "FbxApp.h"
#include <GLM/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLCORE/gl_core_4_4.h>
#include <FBX/FBXFile.h>
#include <Camera/FlyCamera.h>

void FbxApp::InitCamera() {
    camera = new FlyCamera( 1.f );
    camera->SetPerspective( glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f );
    camera->SetLookAt( vec3( 10, 10, 10 ), vec3( 0 ), vec3( 0, 1, 0 ) );
}

bool FbxApp::BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader ) {
    using namespace std;

    int error = GL_FALSE;
    unsigned int vertexShader = LoadShader( pathToVertexShader, GL_VERTEX_SHADER );
    unsigned int fragmentShader = LoadShader( pathToFragmentShader, GL_FRAGMENT_SHADER );

    if ( vertexShader == 0 && fragmentShader == 0 ) {
        if ( vertexShader == 0 ) { cout << "Failed to load Vertex Shader!" << endl; }
        if ( fragmentShader == 0 ) { cout << "Failed to load Fragment Shader!" << endl; }
        return false;
    }

    programID = glCreateProgram();
    glAttachShader( programID, vertexShader );
    glAttachShader( programID, fragmentShader );
    glLinkProgram( programID );

    glGetProgramiv( programID, GL_LINK_STATUS, &error );
    if ( error == GL_FALSE ) {
        int infoLogSize = 0;
        glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogSize );
        char* infoLog = new char[infoLogSize];
        glGetProgramInfoLog( programID, infoLogSize, 0, infoLog );
        printf( "Error: Failed to link shader program!\n" );
        printf( "%s\n", infoLog );
        delete[]( infoLog );
        return false;
    }

    glDeleteShader( fragmentShader );
    glDeleteShader( vertexShader );
    return true;
}

unsigned int FbxApp::LoadShader( const char* pathToShader, unsigned int shaderType ) {
    using namespace std;
    unsigned int shaderHandle = glCreateShader( shaderType );

    ifstream shader;
    shader.open( pathToShader );
    if ( shader.is_open() ) {
        string line;
        stringstream stream;
        while ( getline( shader, line ) ) {
            stream << line << endl;
        }
        char* source = new char[stream.str().length() + 1];
        strcpy_s( source, stream.str().length() + 1, stream.str().c_str() );

        cout << source << endl;

        glShaderSource( shaderHandle, 1, (const char**)&source, 0 );
        glCompileShader( shaderHandle );

        shader.close();
    } else {
        cout << "Could not load Shader: " << pathToShader << endl;
        glDeleteShader( shaderHandle );
        return 0;
    }
    return shaderHandle;
}

void FbxApp::Init_Logic() {
    InitCamera();

    if ( BuildShaders( "./shader/Vertex.glsl", "./shader/Fragment.glsl" ) == false ) {
        std::cout << "Failed to build shaders!" << std::endl;
    }

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ibo );

    int imageWidth = 0, imageHeight = 0, imageFormat = 0;
    unsigned char* imageData = stbi_load( "./textures/crate.png", &imageWidth, &imageHeight, &imageFormat, STBI_default );

    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    stbi_image_free( imageData );
}

void FbxApp::FixedUpdate_Logic() {
    camera->Update();
}

void FbxApp::Render_Logic() {
    float vertexData[] {
        -5, 0, 5, 1, 0, 1,
        5, 0, 5, 1, 1, 1,
        5, 0, -5, 1, 1, 0,
        -5, 0, -5, 1, 0, 0,
    };

    unsigned int indexData[] {
        0, 1, 2,
        0, 2, 3,
    };

    glBindVertexArray( vao );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, vertexData, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * 3 * 2, indexData, GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*)( sizeof( float ) * 4 ) );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glUseProgram( programID );

    camera->UpdateProjectionViewTransform();
    glUniformMatrix4fv( glGetUniformLocation( programID, "uProjectionView" ),
                        1, GL_FALSE, glm::value_ptr( camera->Camera_View_Transform_Mat4 ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureID );

    glUniform1i( glGetUniformLocation( programID, "uDiffuse" ), 0 );

    glBindVertexArray( vao );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

void FbxApp::Shutdown_Logic() {
}

FbxApp::FbxApp() :
    FbxApp( "FbxApp" ) {}

FbxApp::FbxApp( const std::string set_name ) :
    FbxApp( set_name, 1280, 720 ) {}

FbxApp::FbxApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ),
    vao( 0 ), vbo( 0 ), ibo( 0 ), programID( 0 ),
    textureID( 0 ), camera( nullptr ) {}

FbxApp::~FbxApp() {}
