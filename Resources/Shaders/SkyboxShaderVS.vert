//Asta e shader-ul folosit doar pe obiectul de tip skybox 

attribute vec3 a_posL;
attribute vec3 a_colL;

varying vec3 v_color;
varying vec3 v_texCoord;

uniform mat4 u_mvp;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;

	v_color = a_colL;
	v_texCoord = a_posL;
}
   