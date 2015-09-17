#include "LightApp.h"
#include <GLM/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLCORE/gl_core_4_4.h>
#include <FBX/FBXFile.h>
#include <Camera/FlyCamera.h>

void LightApp::InitCamera() {
    camera = new FlyCamera( 1.f );
    camera->SetPerspective( glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f );
    camera->SetLookAt( vec3( 10, 10, 10 ), vec3( 0 ), vec3( 0, 1, 0 ) );
}

bool LightApp::BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader ) {
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

unsigned int LightApp::LoadShader( const char* pathToShader, unsigned int shaderType ) {
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

void LightApp::Init_Logic() {
    InitCamera();

    if ( BuildShaders( "./shader/Vertex.glsl", "./shader/Fragment.glsl" ) == false ) {
        std::cout << "Failed to build shaders!" << std::endl;
    }

    mainObject = new ObjectData::Object( "./assets/soulspear/soulspear.fbx" );
}

void LightApp::FixedUpdate_Logic() {
    camera->Update();
}

void LightApp::Render_Logic() {
    if( mainObject != nullptr ) {
        camera->UpdateProjectionViewTransform();
        mainObject->Render( programID, camera->Camera_View_Transform_Mat4 );
    }
}

void LightApp::Shutdown_Logic() {
    if ( mainObject != nullptr ) {
        delete( mainObject );
    }
}

LightApp::LightApp() :
    LightApp( "LightApp" ) {}

LightApp::LightApp( const std::string set_name ) :
    LightApp( set_name, 1280, 720 ) {}

LightApp::LightApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ),
    programID( 0 ), camera( nullptr ), mainObject( nullptr ) {}

LightApp::~LightApp() {}
