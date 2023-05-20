precision mediump float;

varying vec3 v_color;
varying vec3 v_norm;
varying vec3 v_Wpos;

uniform vec3 u_objectColor;
uniform vec3 u_cameraPos;

uniform float u_ambientFactor;
uniform float u_specularFactor;
uniform float u_diffuseFactor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

void main()
{
    vec3 ambient = u_ambientFactor * u_lightColor;

    vec3 norm = normalize(v_norm);
    vec3 lightDir = normalize(u_lightPos - v_Wpos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_diffuseFactor * diff * u_lightColor;

    vec3 viewDir = normalize(u_cameraPos - v_Wpos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
    vec3 specular = u_specularFactor * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    //vec3 result = ambient  * u_objectColor;
    //gl_FragColor = vec4(diff,diff,diff, 1.0);
    //gl_FragColor = vec4(v_norm, 1.0);
    gl_FragColor = vec4(result, 1.0);
}
