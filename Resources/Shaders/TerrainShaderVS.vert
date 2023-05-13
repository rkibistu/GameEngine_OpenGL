attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec2 a_texCoord;
attribute vec2 a_texCoord2;

varying vec3 v_color;
varying vec2 v_texCoord;
varying vec2 v_texCoord2;

uniform mat4 u_mvp;

uniform mat4 u_rotate;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_texCoord = a_texCoord;
	v_texCoord2 = a_texCoord2;
}
   