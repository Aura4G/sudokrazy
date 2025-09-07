#version 120 core

uniform sampler2D texture; //the texture from SFML
uniform float brightness; //the brightness factor
uniform float contrast; //the contrast factor

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy); //Each pixel is likened to a vec4
    pixel.rgb *= brightness; //Scale color channels by brightness factor. To display regular colours, brightness is 1.f
    float average = (pixel.r + pixel.g + pixel.b) / 3.0;
    pixel.r = average + (pixel.r - average) * contrast;
    pixel.g = average + (pixel.g - average) * contrast;
    pixel.b = average + (pixel.b - average) * contrast;
    gl_FragColor = pixel; //The fragment's color is now the newly shaded pixel
}