#include "RenderApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Vertex.h"
#include <GLM/ext.hpp>
#include <GLCORE/gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <Camera/FlyCamera.h>

void RenderApp::GenerateGrid( unsigned rows, unsigned cols ) {
    Vertex* arrVertex = new Vertex[rows* cols];
    for ( unsigned int r = 0; r < rows; ++r ) {
        for ( unsigned int c = 0; c < cols; ++c ) {
            unsigned int n = r*cols + c;
            arrVertex[n].position = glm::vec4( float( c ), 0, float( r ), 1 );
            glm::vec3 color = glm::vec3( sinf( ( c / float( cols - 1 ) )*( r / float( rows - 1 ) ) ) );
            arrVertex[n].color = glm::vec4( color, 1 );
        }
    }

    unsigned int* arrIndex = new unsigned int[( rows - 1 ) * ( cols - 1 ) * 6];
    unsigned int n = 0;
    for ( unsigned int r = 0; r < ( rows - 1 ); ++r ) {
        for ( unsigned int c = 0; c < ( cols - 1 ); ++c ) {
            arrIndex[n++] = r * cols + c;
            arrIndex[n++] = ( r + 1 ) * cols + c;
            arrIndex[n++] = ( r + 1 ) * cols + ( c + 1 );

            arrIndex[n++] = r * cols + c;
            arrIndex[n++] = ( r + 1 ) * cols + ( c + 1 );
            arrIndex[n++] = r * cols + ( c + 1 );
        }
    }

    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, ( rows * cols ) * sizeof( Vertex ), arrVertex, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)( sizeof( glm::vec4 ) ) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, ( rows - 1 ) * ( cols - 1 ) * 6 * sizeof( unsigned int ), arrIndex, GL_STATIC_DRAW );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glUseProgram( programID );
    unsigned int projectionViewUniform = glGetUniformLocation( programID, "ProjectionView" );
    glUniformMatrix4fv( projectionViewUniform, 1, false, glm::value_ptr( camera->Camera_View_Transform_Mat4 ) );
    unsigned int timeUniform = glGetUniformLocation( programID, "Time" );
    glUniform1f( timeUniform, float( glfwGetTime() ) );

    glBindVertexArray( vao );
    unsigned int indexCount = ( rows - 1 ) * ( cols - 1 ) * 6;
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0 );

    delete[]( arrIndex );
    delete[]( arrVertex );
}

void RenderApp::AfterInit() {
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ibo );
    glGenVertexArrays( 1, &vao );

    if ( BuildShaders( "./shader/Vertex.glsl", "./shader/Fragment.glsl" ) == false ) {
        std::cout << "Failed to build shaders!" << std::endl;
    }

    camera = new FlyCamera( 1.f );
    camera->SetPerspective( glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f );
    camera->SetLookAt( vec3( 10, 10, 10 ), vec3( 0 ), vec3( 0, 1, 0 ) );
}

void RenderApp::BeforeUpdate() {
    camera->Update();
}

void RenderApp::BeforeRender() {
    camera->UpdateProjectionViewTransform();
    GenerateGrid( 10, 10 );
}

void RenderApp::BeforeShutdown() {
    glDeleteProgram( programID );
}

bool RenderApp::BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader ) {
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

unsigned int RenderApp::LoadShader( const char* pathToShader, unsigned int shaderType ) {
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

RenderApp::RenderApp() :
    RenderApp( "TEST" ) {}

RenderApp::RenderApp( const std::string set_name ) :
    RenderApp( set_name, 1280, 720 ) {}

RenderApp::RenderApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ),
    vao( 0 ), vbo( 0 ), ibo( 0 ),
    programID( 0 ), camera( nullptr ) {}

RenderApp::~RenderApp() {}
