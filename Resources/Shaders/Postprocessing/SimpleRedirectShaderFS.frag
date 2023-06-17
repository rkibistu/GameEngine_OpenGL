precision mediump float;

varying vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{    
    vec4 tex = texture2D(u_Texture, TexCoords);
    gl_FragColor = vec4(tex);
   // gl_FragColor = vec4(1.0);
}  