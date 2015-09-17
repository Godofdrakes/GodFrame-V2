#version 410

layout(location=0) in vec4 inPosition;
layout(location=1) in vec2 inTexCoord;

out vec2 vTexCoord;

uniform mat4 uProjectionView;

void main() {
    vTexCoord = inTexCoord;
    gl_Position = uProjectionView * inPosition;
}
