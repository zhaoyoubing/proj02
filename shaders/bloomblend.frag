// bloomblend.frag
#version 430 core

in vec2 texCoord;

layout (binding=1) uniform sampler2D texrender;
layout (binding=2) uniform sampler2D texblur;

out vec4 colour_out;

void main()
{             
    vec3 renderColour = texture(texrender, texCoord).rgb;      
    vec3 bloomColour = texture(texblur, texCoord).rgb;
    
    // additive blending
    vec3 blendColour = renderColour + bloomColour; 
    
    // set "result = bloomColour" to test blurring
    // vec3 result = bloomColour;
    vec3 result = blendColour;

    // =============================================
    // exposure tone mapping https://learnopengl.com/Advanced-Lighting/HDR
    /*
    const float gamma = 2.2;
    float exposure = 1.0;
    result = vec3(1.0) - exp(-result * exposure);
    // Gamma correction       
    result = pow(result, vec3(1.0 / gamma));
    */
    
    colour_out = vec4(result, 1.0);
}  