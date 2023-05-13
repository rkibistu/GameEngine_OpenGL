precision mediump float;


varying vec3 v_color;
varying vec2 v_texCoord;
varying vec2 v_texCoord2;

uniform sampler2D u_Texture;

void main()
{
	//gl_FragColor = vec4(v_color, 1.0);

	vec4 blend = texture2D(u_Texture, v_texCoord);
	vec4 tex = texture2D(u_Texture, v_texCoord);

	

	gl_FragColor = blend;
}
