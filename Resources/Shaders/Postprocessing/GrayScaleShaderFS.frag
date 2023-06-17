precision mediump float;

varying vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{    
    vec4 tex = texture2D(u_Texture, TexCoords);
    
    vec3 weights = vec3(0.2,0.5,0.3);
    tex.x = (tex.x * 0.2 + tex.y * 0.5 + tex.z * 0.3);
    tex.y = tex.x;
    tex.z = tex.x;
    gl_FragColor = vec4(tex);
}   