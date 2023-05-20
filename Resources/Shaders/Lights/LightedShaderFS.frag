precision mediump float;

varying vec3 v_color;
varying vec3 v_norm;
varying vec3 v_Wpos;
varying vec2 v_texCoord;

uniform sampler2D u_Texture;

uniform vec3 u_objectColor;
uniform vec3 u_cameraPos;

uniform float u_ambientFactor;
uniform float u_specularFactor;
uniform float u_diffuseFactor;

struct Light{

    int type;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 position; //used for point or spot
    vec3 direction; //used for directional
    float spotAngle;
};
uniform Light u_lights[8];
uniform  int u_lightsCount;

uniform vec3 u_ambientColor;
uniform float u_ambientRatio;

void main() {

    vec4 texColor = texture2D(u_Texture, v_texCoord);
	if(texColor.a < 0.2)
		discard;

    //Calculate LIGHTS
    vec3 ambientColor = u_ambientRatio * u_ambientColor;

    vec3 diffuseColorSum = vec3(0.0,0.0,0.0);
    vec3 specularColorSum = vec3(0.0,0.0,0.0);
    for(int i=0;i<8;i++){

        if(i >= u_lightsCount)
            break;

        vec3 diffuseColor = u_lights[i].diffuseColor;
        vec3 specularColor = u_lights[i].specularColor;
        vec3 lightPos = u_lights[i].position;

        vec3 lightDir;
        float fallout = 1.0;
        if (u_lights[i].type == 0){
            //point
            lightDir = normalize(lightPos - v_Wpos);  
        }
        else if (u_lights[i].type == 1){
            // directional
            lightDir = normalize(-u_lights[i].direction); 
        }
        else if (u_lights[i].type == 2){
            //spot
            vec3 spotDir = normalize(-u_lights[i].direction);
            float u = dot(spotDir, normalize(lightPos - v_Wpos));
            if( u > cos(u_lights[i].spotAngle)){

                lightDir = normalize(lightPos - v_Wpos);
                fallout = smoothstep(u_lights[i].spotAngle, 0.0, acos(u));
               
            }
            else{
                lightDir = vec3(0.0);
                fallout = 0.0;
            }
            
                // Decomenteaza asta sa vezi efectiv efectul SPOT
            //gl_FragColor = vec4(fallout,fallout,fallout,1.0);
            //  return;

        }

        //diffuse
        vec3 norm = normalize(v_norm);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = u_diffuseFactor * diff * diffuseColor * fallout;
        diffuseColorSum += diffuse;
        
        //specualr
        vec3 viewDir = normalize(u_cameraPos - v_Wpos);
        vec3 reflectDir = reflect(-lightDir,norm);
        float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
        vec3 specular = u_specularFactor * spec * diffuseColor * fallout;
        specularColorSum+= specular;
    }
    vec4 result = vec4((ambientColor + diffuseColorSum + specularColorSum),1.0) * texColor;

    //Fog


    //Skybox



    gl_FragColor = result;
}
