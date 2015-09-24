#include "FrameApp.h"
#include <GLM/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLCORE/gl_core_4_4.h>
#include <FBX/FBXFile.h>
#include <Camera/FlyCamera.h>

void FrameApp::InitCamera() {
    camera = new FlyCamera( 1.f );
    camera->SetPerspective( glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f );
    camera->SetLookAt( vec3( 10, 10, 10 ), vec3( 0 ), vec3( 0, 1, 0 ) );
}

bool FrameApp::BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader ) {
    using namespace std;

    int error = GL_FALSE;
    unsigned int vertexShader = LoadShader( pathToVertexShader, GL_VERTEX_SHADER );
    unsigned int fragmentShader = LoadShader( pathToFragmentShader, GL_FRAGMENT_SHADER );

    if ( vertexShader == 0 && fragmentShader == 0 ) {
        if ( vertexShader == 0 ) {
            cout << "Failed to load Vertex Shader!" << endl;
        }
        if ( fragmentShader == 0 ) {
            cout << "Failed to load Fragment Shader!" << endl;
        }
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

unsigned int FrameApp::LoadShader( const char* pathToShader, unsigned int shaderType ) {
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

        glShaderSource( shaderHandle, 1, ( const char** )&source, 0 );
        glCompileShader( shaderHandle );

        shader.close();
    } else {
        cout << "Could not load Shader: " << pathToShader << endl;
        glDeleteShader( shaderHandle );
        return 0;
    }
    return shaderHandle;
}

void FrameApp::Init_Logic() {
    InitCamera();

    if ( BuildShaders( "./shader/Vertex.glsl", "./shader/Fragment.glsl" ) == false ) {
        std::cout << "Failed to build shaders!" << std::endl;
    }

    mainObject = new Object( "./assets/soulspear/soulspear.fbx" );
    
    glGenFramebuffers( 1, &frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );

    glGenTextures( 1, &frameTexture );
    glBindTexture( GL_TEXTURE_2D, frameTexture );

    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameTexture, 0 );

    glGenRenderbuffers( 1, &frameDepth );
    glBindRenderbuffer( GL_RENDERBUFFER, frameDepth );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512 );

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameDepth );

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, drawBuffers );

    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if( status != GL_FRAMEBUFFER_COMPLETE ) {
        std::cout << "Framebuffer Error!" << std::endl;
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    float vertexData[] = {
        -5, 0, -5, 1, 0, 0,
        5, 0, -5, 1, 1, 0,
        5, 10, -5, 1, 1, 1,
        -5, 10, -5, 1, 0, 1,
    };

    unsigned int indexData[] = {
        0, 1, 2,
        0, 2, 3,
    };

    glGenVertexArrays( 1, &mirrorVAO );
    glBindVertexArray( mirrorVAO );
    glGenBuffers( 1, &mirrorVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mirrorVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, vertexData, GL_STATIC_DRAW );
    glGenBuffers( 1, &mirrorIBO );
    glBindBuffer( 1, mirrorIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * 6, indexData, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*)( sizeof( float ) * 4 ) );
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void FrameApp::FixedUpdate_Logic() {
    camera->Update();
}

void FrameApp::Render_Logic() {
    if( mainObject != nullptr ) {
        camera->UpdateProjectionViewTransform();
        mainObject->Render( programID, camera->Camera_View_Transform_Mat4, camera->Position_Vec3 );
    }
}

void FrameApp::Shutdown_Logic() {
    glDeleteRenderbuffers( 1, &frameDepth );
    glDeleteTextures( 1, &frameTexture );
    glDeleteFramebuffers( 1, &frameBuffer );
    if ( mainObject != nullptr ) {
        delete( mainObject );
    }
}

FrameApp::FrameApp() :
    FrameApp( "FrameApp" ) {}

FrameApp::FrameApp( const std::string set_name ) :
    FrameApp( set_name, 1280, 720 ) {}

FrameApp::FrameApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ),
    programID( 0 ), camera( nullptr ), mainObject( nullptr ) {}

FrameApp::~FrameApp() {}
