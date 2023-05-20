precision mediump float;

varying vec3 v_color;
varying vec2 v_texCoord;
varying vec4 v_Wpos;
varying vec4 v_Wnorm;

uniform sampler2D u_Texture;
uniform samplerCube u_TextureCube;
uniform float u_factorTexture;
uniform float u_factorReflect;
uniform float u_fogNear;
uniform float u_fogFar;
uniform vec3 u_fogColor;

uniform vec3 u_cameraPos;

void main()
{
	//gl_FragColor = vec4(v_color, 1.0);

	vec4 texColor = texture2D(u_Texture, v_texCoord);
	if(texColor.a < 0.2)
		discard;

	vec3 vectCam = vec3(v_Wpos) - u_cameraPos;
	vec3 dirReflect = reflect(
		normalize(vectCam),
		normalize(vec3(v_Wnorm))
		);
	vec4 texColorCube = textureCube(u_TextureCube, dirReflect);

	float dist = distance(vec3(v_Wpos), u_cameraPos);
	float alpha = (dist - u_fogNear)/(u_fogFar - u_fogNear);
	alpha = clamp(alpha,0.0,1.0);

	vec4 culoareObiect =  texColor * u_factorTexture + texColorCube * u_factorReflect;
	gl_FragColor = alpha * vec4(u_fogColor,1.0) + (1.0 - alpha) * culoareObiect;
	
}
