precision mediump float;

varying vec3 v_color;
varying vec3 v_norm;
varying vec3 v_Wpos;

uniform vec3 u_objectColor;
uniform vec3 u_cameraPos;

uniform float u_ambientFactor;
uniform float u_specularFactor;
uniform float u_diffuseFactor;

struct Light{

    int type;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 position;
};
uniform Light u_lights[8];
uniform vec3 u_ambientColor;
uniform float u_ambientRatio;

void main() {

    vec3 diffuseColor = u_lights[0].diffuseColor;
    vec3 specularColor = u_lights[0].specularColor;
    vec3 lightPos = u_lights[0].position;

    vec3 ambient = u_ambientRatio * u_ambientColor;

    vec3 norm = normalize(v_norm);
    vec3 lightDir = normalize(lightPos - v_Wpos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_diffuseFactor * diff * diffuseColor;

    vec3 viewDir = normalize(u_cameraPos - v_Wpos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
    vec3 specular = u_specularFactor * spec * diffuseColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    gl_FragColor = vec4(result, 1.0);
}
