#version 410

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inColor;
layout( location = 2 ) in vec4 inNormal;
layout( location = 3 ) in vec4 inTangent;
layout( location = 4 ) in vec4 inBiNormal;
layout( location = 5 ) in vec4 inIndices;
layout( location = 6 ) in vec4 inWeights;
layout( location = 7 ) in vec2 inTexCoord1;
layout( location = 8 ) in vec2 inTexCoord2;

out vec4 vPosition;
out vec4 vColor;
out vec4 vNormal;
out vec4 vTangent;
out vec4 vBiNormal;
out vec4 vIndices;
out vec4 vWeights;
out vec2 vTexCoord1;
out vec2 vTexCoord2;

uniform mat4 uProjectionView;

void main() {
    vPosition = inPosition;
    vColor = inColor;
    vNormal = inNormal;
    vTangent = inTangent;
    vBiNormal = inBiNormal;
    vIndices = inIndices;
    vWeights = inWeights;
    vTexCoord1 = inTexCoord1;
    vTexCoord2 = inTexCoord2;
    gl_Position = uProjectionView * inPosition;
}
