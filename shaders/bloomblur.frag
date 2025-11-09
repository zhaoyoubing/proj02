#version 430
  
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform int pass;

layout (binding=1)  uniform sampler2D tex0;
//layout (binding=2)  uniform sampler2D tex1;
layout (binding=5)  uniform sampler2D tex2;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 colour_out;

void main()
{   
    vec3 result=vec3(0.0);         
    if (mod(pass,2) == 0)
    {
        vec2 tex_offset = 1.0 / textureSize(tex2, 0); // gets size of single texel
        result = texture(tex2, texCoord).rgb * weight[0]; // current fragment's contribution

        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex2, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(tex2, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }

        //result = texture(tex2, texCoord).rgb;
    }
    else
    {
        vec2 tex_offset = 1.0 / textureSize(tex2, 0); // gets size of single texel
        result = texture(tex2, texCoord).rgb * weight[0]; // current fragment's contribution
        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex2, texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(tex2, texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }

        //result = texture(tex2, texCoord).rgb;
    }

    colour_out = vec4(result, 1.0);
}