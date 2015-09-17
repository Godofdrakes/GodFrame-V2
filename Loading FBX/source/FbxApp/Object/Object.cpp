#include "Object.h"
#include <GLCORE/gl_core_4_4.h>
#include <FBX/FBXFile.h>
#include <iostream>
#include <GLM/gtc/type_ptr.hpp>

namespace ObjectData {

    // Not needed by classes other than object
    struct Vert {
        glm::vec4 position;
        glm::vec2 texCoord;
    };

    Object::Object( const char* fbxFilePath ) :
        vao( 0 ), vbo( 0 ), ibo( 0 ), textureID( nullptr ), textureCount( 0 ) {

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
        Vert* verts = new Vert[mesh->m_vertices.size()];
        for ( unsigned int n = 0; n < mesh->m_vertices.size(); ++n ) {
            verts[n].position = mesh->m_vertices[n].position;
            verts[n].texCoord = mesh->m_vertices[n].texCoord1;
        }

        glBindVertexArray( vao );

        // VBO handling
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( Vert ) * mesh->m_vertices.size(), verts, GL_STATIC_DRAW );

        // IBO handling
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * mesh->m_indices.size(), mesh->m_indices.data(), GL_STATIC_DRAW );
        indexCount = mesh->m_indices.size();

        // Position
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vert ), nullptr );
        // Texture coord
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Vert ), ( void* )sizeof( glm::vec4 ) );

        // Done buffering data
        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        delete[]( verts );
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
                         const unsigned int diffuse ) {
        // If there's more than one mesh this won't render those.
        // TODO: FIX?
        glUseProgram( program_id );

        glUniformMatrix4fv( glGetUniformLocation( program_id, "uProjectionView" ),
                            1, GL_FALSE, glm::value_ptr( projection_view ) );

        glUniform1i( glGetUniformLocation( program_id, "uDiffuse" ), diffuse );

        if ( textureCount > 0 && textureID != nullptr ) {
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, textureID[0] );
        }

        glBindVertexArray( vao );
        if ( textureCount == 0 ) { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); }
        glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr );

        if ( textureCount == 0 ) { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }
        glBindVertexArray( 0 );
    }

}