#version 410

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inColor;
layout( location = 2 ) in vec4 inNormal;
layout( location = 3 ) in vec4 inTangent;
layout( location = 4 ) in vec4 inBiNormal;
layout( location = 5 ) in vec4 inIndices;
layout( location = 6 ) in vec4 inWeights;
layout( location = 7 ) in vec2 inTexCoord0;
layout( location = 8 ) in vec2 inTexCoord1;

out vec4 vPosition;
out vec4 vColor;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vIndices;
out vec4 vWeights;
out vec2 vTexCoord0;
out vec2 vTexCoord1;

uniform mat4 uProjectionView;

void main() {
    vPosition = inPosition;
    vColor = inColor;
    vNormal = inNormal.xyz;
    vTangent = inTangent.xyz;
    vBiTangent = cross( vNormal, vTangent );
    vIndices = inIndices;
    vWeights = inWeights;
    vTexCoord0 = inTexCoord0;
    vTexCoord1 = inTexCoord1;
    gl_Position = uProjectionView * inPosition;
}
