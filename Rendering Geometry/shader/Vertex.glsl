#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;

out vec4 vColor;

uniform mat4 ProjectionView;
uniform float Time;

void main() {
    vColor = Color;
    vec4 pos = Position;
    pos.y = sin( Time + Position.x );
    gl_Position = ProjectionView * pos;
}