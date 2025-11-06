// simpledepth.frag
#version 410

out vec4 fragColour;

void main()
{   
    // code to show the depth buffer, can be commented
    float d = gl_FragCoord.z;
    fragColour = vec4(d, d, d, 1.0);
} 