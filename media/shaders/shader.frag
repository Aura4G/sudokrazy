#version 120 core

uniform sampler2D texture; // The texture from SFML
uniform float brightness; // The brightness factor
uniform float contrast; // The contrast factor
uniform float hue; // The hue factor

vec3 hueShift(vec3 colour, float hueAngle)
{
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hueAngle);
    return colour * cosAngle + cross(k, colour) * sin(hueAngle) + k * dot(k, colour) * (1.0 - cosAngle);
}

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);   // Each pixel is likened to a vec4
    pixel.rgb *= brightness;                              // Scale color channels by brightness factor. To display regular colours, brightness is 1.f
    
    pixel.rgb = hueShift(pixel.rgb, hue * 6.28318530718); // Change hue

    float average = (pixel.r + pixel.g + pixel.b) / 3.0;

    pixel.r = average + (pixel.r - average) * contrast;
    pixel.g = average + (pixel.g - average) * contrast;
    pixel.b = average + (pixel.b - average) * contrast;

    gl_FragColor = pixel;                                 // The fragment's color is now the newly shaded pixel
}