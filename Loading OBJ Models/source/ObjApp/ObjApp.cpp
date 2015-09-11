#include "ObjApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GLM/ext.hpp>
#include <GLCORE/gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <Camera/FlyCamera.h>

bool ObjApp::BuildShaders( const char* pathToVertexShader, const char* pathToFragmentShader ) {
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

unsigned int ObjApp::LoadShader( const char* pathToShader, unsigned int shaderType ) {
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

void ObjApp::CreateOpenGLBuffers( std::vector<tinyobj::shape_t>& shapes ) {
    glInfo.resize( shapes.size() );

    for ( unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index ) {
        glGenVertexArrays( 1, &glInfo[mesh_index].vao );
        glGenBuffers( 1, &glInfo[mesh_index].vbo );
        glGenBuffers( 1, &glInfo[mesh_index].ibo );
        glBindVertexArray( glInfo[mesh_index].vao );

        unsigned int float_count = shapes[mesh_index].mesh.positions.size();
        float_count += shapes[mesh_index].mesh.normals.size();
        float_count += shapes[mesh_index].mesh.texcoords.size();

        std::vector<float> vertex_data;
        vertex_data.reserve( float_count );
        vertex_data.insert( vertex_data.end(), shapes[mesh_index].mesh.positions.begin(), shapes[mesh_index].mesh.positions.end() );
        vertex_data.insert( vertex_data.end(), shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end() );

        glInfo[mesh_index].indexCount = shapes[mesh_index].mesh.indices.size();

        glBindBuffer( GL_ARRAY_BUFFER, glInfo[mesh_index].vbo );
        glBufferData( GL_ARRAY_BUFFER, vertex_data.size() * sizeof( float ), vertex_data.data(), GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glInfo[mesh_index].ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      shapes[mesh_index].mesh.indices.size() * sizeof( unsigned int ),
                      shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW );

        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_TRUE, 0, (void*)( sizeof( float )*shapes[mesh_index].mesh.positions.size() ) );

        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
}

void ObjApp::DrawOpenGLBuffers( std::vector<tinyobj::shape_t>& shapes ) {
    glUseProgram( programID );
    int projectionUniform = glGetUniformLocation( programID, "ProjectionView" );
    glUniformMatrix4fv( projectionUniform, 1, GL_FALSE, (float*)&camera->Camera_View_Transform_Mat4 );
    for ( unsigned int n = 0; n < glInfo.size(); ++n ) {
        glBindVertexArray( glInfo[n].vao );
        glDrawElements( GL_TRIANGLES, glInfo[n].indexCount, GL_UNSIGNED_INT, 0 );
    }
}

void ObjApp::AfterInit() {
    camera = new FlyCamera( 1.f );
    camera->SetPerspective( glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f );
    camera->SetLookAt( vec3( 10, 10, 10 ), vec3( 0 ), vec3( 0, 1, 0 ) );

    if ( BuildShaders( "./shader/Vertex.glsl", "./shader/Fragment.glsl" ) == false ) {
        std::cout << "Failed to build shaders!" << std::endl;
    }

    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ibo );
    glGenVertexArrays( 1, &vao );

    std::string err = tinyobj::LoadObj( shapes, materials, "./models/bunny.obj" );
    printf( err.c_str() );
    printf( "\n" );

    CreateOpenGLBuffers( shapes );
}

void ObjApp::BeforeUpdate() {
    camera->Update();
}

void ObjApp::BeforeRender() {
    camera->UpdateProjectionViewTransform();
    DrawOpenGLBuffers( shapes );
}

void ObjApp::BeforeShutdown() {
    glDeleteProgram( programID );
}

ObjApp::ObjApp() :
    ObjApp("TEST") {}

ObjApp::ObjApp( const std::string set_name ) :
    ObjApp( set_name, 1280, 720 ) {}

ObjApp::ObjApp( const std::string set_name, const int set_width, const int set_height ) :
    Application( set_name, set_width, set_height ),
    vao(0), vbo(0), ibo(0),
    programID(0), camera(nullptr) {}

ObjApp::~ObjApp() {}
