// bloomfilter.frag
#version 430

in vec2 texCoord;

// render-to-texture of the scene generated in the 1st pass
layout (binding=1) uniform sampler2D renderTex;

out vec4 colour_out;

void main()
{
    vec4 colour = texture(renderTex, texCoord);

    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(colour.rgb, vec3(0.2126, 0.7152, 0.0722));

    // filter the bright part based on a threshold
    if(brightness > 0.9)
        colour_out = vec4(colour.rgb, 1.0);
    else
        colour_out = vec4(0.0, 0.0, 0.0, 1.0);
}