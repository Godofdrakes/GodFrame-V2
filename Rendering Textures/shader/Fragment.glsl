#version 410

in vec2 vTexCoord;

out vec4 outFragColor;

uniform sampler2D uDiffuse;

void main() {
    outFragColor = texture( uDiffuse, vTexCoord );
}
