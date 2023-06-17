precision mediump float;

varying vec2 v_texCoord;

uniform sampler2D u_Texture;
uniform float u_blurStep;

void main()
{    
    vec4 sample0, sample1, sample2, sample3;

    float step = u_blurStep / 100.0;
    sample0 = texture2D(u_Texture,
                        vec2(v_texCoord.x - step,
                             v_texCoord.y - step));
    sample1 = texture2D(u_Texture,
                        vec2(v_texCoord.x + step,
                             v_texCoord.y + step));    
    sample2 = texture2D(u_Texture,
                        vec2(v_texCoord.x + step,
                             v_texCoord.y - step));
    sample3 = texture2D(u_Texture,
                        vec2(v_texCoord.x - step,
                             v_texCoord.y + step));

    gl_FragColor = (sample0 + sample1 + sample2 + sample3)/4.0;
}   