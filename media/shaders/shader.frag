#version 120 core

uniform sampler2D texture; //the texture from SFML
uniform float brightness; //the brightness factor
uniform float contrast; //the contrast factor

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy); //Each pixel is likened to a vec4
    pixel.rgb *= brightness; //Scale color channels by brightness factor. To display regular colours, brightness is 1.f
    float difference;
    if (pixel.r >= pixel.g && pixel.r >= pixel.b)
    {
        difference = pixel.r - pixel.g;
        pixel.g = pixel.r - difference * contrast;
        difference = pixel.r - pixel.b;
        pixel.b = pixel.r - difference * contrast;
    }
    else if (pixel.g >= pixel.r && pixel.g >= pixel.b)
    {
        difference = pixel.g - pixel.r;
        pixel.r = pixel.g - difference * contrast;
        difference = pixel.g - pixel.b;
        pixel.b = pixel.g - difference * contrast;
    }
    else
    {
        difference = pixel.b - pixel.r;
        pixel.r = pixel.b - difference * contrast;
        difference = pixel.b - pixel.g;
        pixel.g = pixel.b - difference * contrast;
    }

    gl_FragColor = pixel; //The fragment's color is now the newly shaded pixel
}