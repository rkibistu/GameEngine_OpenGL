attribute vec3 a_posL;

uniform mat4 u_mvp;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position =   u_mvp * posL;
}
   