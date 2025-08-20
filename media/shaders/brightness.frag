//OpenGL fragment shader for game brightness

uniform sampler2D texture; //the texture from SFML
uniform float brightness; //the brightness factor

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy); //Each pixel is likened to a vec4
    pixel.rgb *= brightness; //Scale color channels by brightness factor. To display regular colours, brightness is 1.f
    gl_FragColor = pixel; //The fragment's color is now the newly shaded pixel
}