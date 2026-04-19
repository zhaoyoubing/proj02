#version 430 core

in vec3 vColor;
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    // gl_PointCoord is a built-in variable ranging from 0 to 1 across the point
    /*
    vec2 coord = gl_PointCoord - vec2(0.5);

    // Create circular points
    float dist = length(coord);

    if (dist > 0.5)
        discard;
    */

    //FragColor = vec4(vColor, 1.0);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}