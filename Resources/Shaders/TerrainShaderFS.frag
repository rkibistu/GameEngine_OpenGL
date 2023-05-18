precision mediump float;


varying vec3 v_color;
varying vec2 v_texCoord;
varying vec2 v_texCoord2;
varying vec4 v_Wpos;

uniform sampler2D u_TextureBlend;
uniform sampler2D u_TextureGrass;
uniform sampler2D u_TextureRock;
uniform sampler2D u_TextureDirt;

uniform vec2 u_deplasament;
uniform float u_countCells;
uniform float u_fogNear;
uniform float u_fogFar;
uniform vec3 u_fogColor;
uniform vec3 u_cameraPos;

void main()
{
	//gl_FragColor = vec4(v_color, 1.0);

	vec2 blendCoord = v_texCoord + u_deplasament * (1.0/u_countCells);
	//vec2 blendCoord = v_texCoord + 1.0/4.0;

	vec4 blend = texture2D(u_TextureBlend, blendCoord);
	vec4 grass = texture2D(u_TextureGrass, v_texCoord2);
	vec4 rock = texture2D(u_TextureRock, v_texCoord2);
	vec4 dirt = texture2D(u_TextureDirt, v_texCoord2);

	float dist = distance(vec3(v_Wpos), u_cameraPos);
	float alpha = (dist - u_fogNear)/(u_fogFar - u_fogNear);
	alpha = clamp(alpha,0.0,1.0);
	
	vec4 final =  blend.x * grass + blend.y * rock + blend.z * dirt;
	gl_FragColor = alpha * vec4(u_fogColor,1.0) + (1.0 - alpha) * final;


	//gl_FragColor = final;
	//gl_FragColor = vec4(u_deplasament,0.0,1.0);
}
