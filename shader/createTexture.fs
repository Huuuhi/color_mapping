#version 330 core

uniform float startHue;
uniform float endHue;
uniform int numOfColor;
uniform int textureWidth;
uniform int textureHeight;

out vec4 fragColor;

void main()
{
    float hueRange = endHue - startHue;
    float hueStep = hueRange / float(numOfColor - 1);
    int colorIndex = int(gl_FragCoord.x) * numOfColor / textureWidth;

    float hue = startHue + float(colorIndex) * hueStep;
    float saturation = 1.0;
    float value = 1.0;

    int h = int(hue * 6);
    float f = hue * 6.0 - float(h);
    float p = value * (1.0 - saturation);
    float q = value * (1.0 - f * saturation);
    float t = value * (1.0 - (1.0 - f) * saturation);

    switch (h)
    {
        case 0:
            fragColor = vec4(value, t, p, 1.0);
            break;
        case 1:
            fragColor = vec4(q, value, p, 1.0);
            break;
        case 2:
            fragColor = vec4(p, value, t, 1.0);
            break;
        case 3:
            fragColor = vec4(p, q, value, 1.0);
            break;
        case 4:
            fragColor = vec4(t, p, value, 1.0);
            break;
        case 5:
            fragColor = vec4(value, p, q, 1.0);
            break;
    }
}
