#version 410

layout(location=0) in vec4 inPosition;
layout(location=1) in vec4 inTexCoord;

out vec4 outTexCoord;

uniform mat4 uProjectionView;

void main() {
    outTexCoord = inTexCoord;
    gl_Position = uProjectionView * inPosition;
}
