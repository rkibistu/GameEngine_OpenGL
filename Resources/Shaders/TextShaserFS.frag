varying vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, TexCoords).r);
    gl_FragColor = vec4(textColor, 1.0) * sampled;
    //gl_FragColor = vec4(1.0f);
}  