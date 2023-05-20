precision mediump float;

varying vec3 v_color;
varying vec2 v_texCoord;
varying vec4 v_Wpos;
varying vec4 v_Wnorm;

uniform sampler2D u_Texture;
uniform sampler2D u_maskTexture;
uniform sampler2D u_displacementTexture;
uniform samplerCube u_TextureCube;

uniform float u_factorTexture;
uniform float u_factorReflect;
uniform float u_fogNear;
uniform float u_fogFar;
uniform vec3 u_fogColor;

uniform vec3 u_cameraPos;

uniform float u_time;
uniform float u_displMax;

void main()
{
	//fire dispalcement
	vec2 texDisplacement = texture2D(u_displacementTexture, vec2(v_texCoord.x, v_texCoord.y + u_time)).rg; 	

	// texDisplacement e culaore, deci e in itnervalul [0,1]
	texDisplacement *= 2.0; //acum e in [0,2]
	texDisplacement -= 1.0; // acum e in [-1,1]
	texDisplacement *= u_displMax; // acum e in [-u_displMax, +u_displMax]

	vec2 uvDisplaced = v_texCoord + texDisplacement; //coordonatele deplasata
	vec4 texColor = texture2D(u_Texture, uvDisplaced);
	vec4 texMask = texture2D(u_maskTexture,v_texCoord); // practic alpha-ul pt foc, din textura mask
	texColor.a *= texMask.r;

	//skyblox reflection
	vec3 vectCam = vec3(v_Wpos) - u_cameraPos;
	vec3 dirReflect = reflect(
		normalize(vectCam),
		normalize(vec3(v_Wnorm))
		);
	vec4 texColorCube = textureCube(u_TextureCube, dirReflect);

	//fog
	float dist = distance(vec3(v_Wpos), u_cameraPos);
	float alpha = (dist - u_fogNear)/(u_fogFar - u_fogNear);
	alpha = clamp(alpha,0.0,1.0);

	vec4 culoareObiect =  texColor * u_factorTexture + texColorCube * u_factorReflect;
	gl_FragColor = alpha * vec4(u_fogColor,culoareObiect.a) + (1.0 - alpha) * culoareObiect;
	
}
