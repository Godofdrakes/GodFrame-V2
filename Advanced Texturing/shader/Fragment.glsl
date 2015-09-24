#version 410

// Input from Vertex shader
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vIndices;
in vec4 vWeights;
in vec2 vTexCoord0;
in vec2 vTexCoord1;

out vec4 outFragColor;

// Is there a way to set these to a default value?
// Would make it possible to check if they were set by the user
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

// Default values for Unforms
uniform vec3 uLightPosition = vec3( 0, 0, 0 );
uniform vec3 uLightColor = vec3( 1, 1, 1 );
uniform vec3 uCameraPosition = vec3( 0, 0, 10 );
uniform float uSpecPower = 128;

void main() {
    vec4 finalColor = vec4( 1, 1, 1, 1 );
    //if( vColor != vec4( 0, 0, 0, 0 ) { finalColor = vColor * finalColor; }

    // Lighting
    mat3 TBN = mat3(
      normalize( vTangent ),
      normalize( vBiTangent ),
      normalize( vNormal )
    );
    vec3 N = texture( uTexture1, vTexCoord1 ).xyz * 2 - 1;
    float d = max( 0, dot( normalize( TBN * N ), normalize( uLightPosition ) ) );

    finalColor = texture( uTexture0, vTexCoord0 ) * finalColor;
    finalColor.rgb = finalColor.rgb * d;

    outFragColor = finalColor;
}
