#include "Object.h"
#include <GLCORE/gl_core_4_4.h>
#include <FBX/FBXFile.h>
#include <iostream>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Object::Object( const char* fbxFilePath ) :
    vao( 0 ), vbo( 0 ), ibo( 0 ),
    textureID( nullptr ), textureCount( 0 ),
    useWireframe( false ) {

    FBXFile* file = new FBXFile();
    file->load( fbxFilePath );
    textureCount = file->getTextureCount();

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ibo );

    if ( textureCount > 0 ) {
        textureID = new unsigned int[textureCount];
        glGenTextures( textureCount, textureID );

        for ( unsigned int n = 0; n < textureCount; ++n ) {
            glBindTexture( GL_TEXTURE_2D, textureID[n] );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, file->getTextureByIndex( n )->width, file->getTextureByIndex( n )->height, 1, GL_RGB, GL_UNSIGNED_BYTE, file->getTextureByIndex( n )->data );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
    }

    FBXMeshNode* mesh = file->getMeshByIndex( 0 );

    glBindVertexArray( vao );

    // VBO handling
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( FBXVertex) * mesh->m_vertices.size(), mesh->m_vertices.data(), GL_STATIC_DRAW );

    // IBO handling
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int) * mesh->m_indices.size(), mesh->m_indices.data(), GL_STATIC_DRAW );
    indexCount = mesh->m_indices.size();

    // inPosition
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::PositionOffset );

    // inColor
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::ColourOffset );

    // inNormal
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::NormalOffset );

    // inTangent
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::TangentOffset );

    // inBiNormal
    glEnableVertexAttribArray( 4 );
    glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::BiNormalOffset );

    // inIndices
    glEnableVertexAttribArray( 5 );
    glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::IndicesOffset );

    // inWeights
    glEnableVertexAttribArray( 6 );
    glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::WeightsOffset );

    // inTexCoord1
    glEnableVertexAttribArray( 7 );
    glVertexAttribPointer( 7, 2, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::TexCoord1Offset );

    // inTexCoord1
    glEnableVertexAttribArray( 8 );
    glVertexAttribPointer( 8, 2, GL_FLOAT, GL_FALSE, sizeof( FBXVertex), ( void* )FBXVertex::TexCoord2Offset );

    // Done buffering data
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    file->unload();
    delete( file );
}

Object::~Object() {
    if ( textureCount > 0 && textureID != nullptr ) {
        glDeleteTextures( textureCount, textureID );
        delete[]( textureID );
        textureID = nullptr;
    }
    glDeleteBuffers( 1, &ibo );
    glDeleteBuffers( 1, &vbo );
    glDeleteVertexArrays( 1, &vao );
    ibo = vbo = vao = 0;
}

void Object::Render( const unsigned int program_id,
                     const glm::mat4& projection_view,
                     const glm::vec3& camera_position ) {
    // If there's more than one mesh this won't render those.
    // TODO: FIX?
    glUseProgram( program_id );

    glUniformMatrix4fv( glGetUniformLocation( program_id, "uProjectionView" ),
                                            1, GL_FALSE, glm::value_ptr( projection_view ) );

    glm::vec3 lightPos = glm::vec3( sin( glfwGetTime() ), 1, cos( glfwGetTime() ) );
    glUniform3fv( glGetUniformLocation( program_id, "uLightPosition" ), 1, glm::value_ptr( lightPos ) );
    glUniform3f( glGetUniformLocation( program_id, "uLightColor" ), 1.f, 1.f, 1.f );
    glUniform3fv( glGetUniformLocation( program_id, "uCameraPosition" ), 1, glm::value_ptr( camera_position ) );
    glUniform1f( glGetUniformLocation( program_id, "uSpecPower" ), 64 );

    for ( unsigned int n = 0; n < textureCount; ++n ) {
        glActiveTexture( GL_TEXTURE0 + n );
        glBindTexture( GL_TEXTURE_2D, textureID[n] );
        std::string uniform = std::string( "uTexture" ) + std::to_string( n );
        glUniform1i( glGetUniformLocation( program_id, uniform.c_str() ), n );
    }

    if ( useWireframe ) { // Set wireframe
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    glBindVertexArray( vao );
    glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr );
    glBindVertexArray( 0 );

    if( useWireframe ) { // Unset wireframe
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}
