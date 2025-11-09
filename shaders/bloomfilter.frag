#version 430

// for lighting
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

layout (binding=1) uniform sampler2D renderTex;

out vec4 colour_out;

void main()
{
    vec4 colour = texture(renderTex, texCoord);

    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(colour.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.9)
        colour_out = vec4(colour.rgb, 1.0);
    else
        colour_out = vec4(0.0, 0.0, 0.0, 1.0);
}