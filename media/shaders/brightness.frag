//OpenGL fragment shader for game brightness

uniform sampler2D texture; //the texture from SFML
uniform float brightness; //the brightness factor

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    pixel.rgb *= brightness; //scale color channels
    gl_FragColor = pixel;
}