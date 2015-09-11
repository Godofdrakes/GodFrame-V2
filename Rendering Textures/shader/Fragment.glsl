#version 410

in vec4 inTexCoord;

out vec4 outFragColor;

uniform sampler2D diffuse;

void main() {
    outFragColor = texture( diffuse, inTexCoord );
}
