attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec3 a_norm;
attribute vec2 a_texCoord;
attribute vec3 a_tgt;
attribute vec3 a_binorm;

varying vec3 v_color;
varying vec3 v_norm;
varying vec3 v_tgt;
varying vec3 v_binorm;
varying vec3 v_Wpos;
varying vec2 v_texCoord;

uniform mat4 u_mvp;
uniform mat4 u_model;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_norm =  mat3(u_model) * a_norm;
	v_tgt =  mat3(u_model) * a_tgt;
	v_binorm =  mat3(u_model) * v_binorm;
	v_Wpos = vec3(u_model * posL);
	v_texCoord = a_texCoord;
}
   