#version 410

in vec4 vPosition;
in vec4 vColor;
in vec4 vNormal;
in vec4 vTangent;
in vec4 vBiNormal;
in vec4 vIndices;
in vec4 vWeights;
in vec2 vTexCoord1;
in vec2 vTexCoord2;

out vec4 outFragColor;

uniform sampler2D uDiffuse;
uniform bool uHasColor;
uniform bool uHasNormal;
uniform bool uHasTangent;
uniform bool uHasBiNormal;
uniform bool uHasIndices;
uniform bool uHasWeights;
uniform bool uHasTexCoord1;
uniform bool uHasTexCoord2;

void main() {
    vec4 myColor = vec4( 1, 1, 1, 1 );
    if( uHasColor ) { myColor = vColor; }
    if( uHasNormal ) {  } // ?
    if( uHasTangent ) {  } // ?
    if( uHasBiNormal ) {  } // ?
    if( uHasIndices ) {  } // ?
    if( uHasWeights ) {  } // ?
    if( uHasTexCoord1 ) { myColor = texture( uDiffuse, vTexCoord1 ) * myColor; }
    if( uHasTexCoord2 ) { } // ?
    outFragColor = myColor;
}
