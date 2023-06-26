precision mediump float;

varying vec2 TexCoords;

uniform sampler2D u_scene;
uniform sampler2D u_bloom;

void main()
{    
    vec4 texScene = texture2D(u_scene, TexCoords);
    vec4 texBloom = texture2D(u_bloom, TexCoords);

    //vec4 blendedColor = texScene * 0.5 + texBloom * 0.5; // You can adjust the blend weights as needed
    vec4 blendedColor = texScene * 1.0 + texBloom * 0.0; // You can adjust the blend weights as needed
    //vec4 blendedColor = texScene * 0.0 + texBloom * 1.0; // You can adjust the blend weights as needed

    gl_FragColor = blendedColor;
}  