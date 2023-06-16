attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec2 a_texCoord;

varying vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(a_posL.xy, 0.0, 1.0);
    TexCoords = a_texCoord;
} 