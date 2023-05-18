precision mediump float;

varying vec3 v_color;
varying vec3 v_texCoord;

uniform samplerCube u_Texture;

void main()
{
	//gl_FragColor = vec4(v_color, 1.0);

	vec4 texColor = textureCube(u_Texture, v_texCoord);

	gl_FragColor = texColor;
}
