//shader ce ia in calcul:
//	textura
//	reflexie skybox
//	ceata

attribute vec3 a_posL;
attribute vec3 a_colL;
attribute vec2 a_texCoord;
attribute vec3 a_norm;

varying vec3 v_color;
varying vec2 v_texCoord;
varying vec4 v_Wpos;
varying vec4 v_Wnorm;

uniform mat4 u_mvp;
uniform mat4 u_model;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_texCoord = a_texCoord;

	v_Wpos = u_model * posL;
	v_Wnorm = u_model * vec4(a_norm,1.0);
}
   