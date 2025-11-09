#version 430 core

in vec2 texCoord;

layout (binding=1) uniform sampler2D renderTex;
layout (binding=2) uniform sampler2D blurTex0;
layout (binding=5) uniform sampler2D blurTex;

out vec4 colour_out;

void main()
{             
    const float gamma = 2.2;
    float exposure = 1.0;

    vec3 color = texture(blurTex, texCoord).rgb; 

    vec3 hdrColor = texture(renderTex, texCoord).rgb;      
    vec3 bloomColor = texture(blurTex, texCoord).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    
    colour_out = vec4(result, 1.0);
    //colour_out = vec4(color, 1.0);
}  