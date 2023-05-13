precision mediump float;

attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec2 a_texCoord;
attribute vec2 a_texCoord2;

varying vec3 v_color;
varying vec2 v_texCoord;
varying vec2 v_texCoord2;

uniform sampler2D u_TextureBlend;
uniform vec3 u_heights;
uniform vec2 u_deplasament;
uniform float u_countCells;

uniform mat4 u_mvp;


void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec2 blendCoord = a_texCoord + u_deplasament * (1.0/u_countCells);
	vec4 blend = texture2D(u_TextureBlend, blendCoord);

	posL.y += blend.x * u_heights.x + blend.y * u_heights.y + blend.z * u_heights.z;

	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_texCoord = a_texCoord;
	v_texCoord2 = a_texCoord2;
}
   