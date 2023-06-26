precision mediump float;

varying vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{    
    vec4 tex = texture2D(u_Texture, TexCoords);
    float brightness = dot(tex.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.4)
        gl_FragColor = vec4(tex.rgb, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(tex);
   // gl_FragColor = vec4(1.0);
}  