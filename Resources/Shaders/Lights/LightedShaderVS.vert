attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec3 a_norm;

varying vec3 v_color;
varying vec3 v_norm;
varying vec3 v_Wpos;

uniform mat4 u_mvp;
uniform mat4 u_model;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_norm =  mat3(u_model) * a_norm;
	v_Wpos = vec3(u_model * posL);
}
   