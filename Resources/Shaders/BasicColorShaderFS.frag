precision mediump float;

varying vec3 v_color;
varying vec2 v_texCoord;

uniform sampler2D u_Texture;

void main()
{
	gl_FragColor = vec4(v_color, 1.0);
}
