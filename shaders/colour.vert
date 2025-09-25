#version 410

in layout(location=0) vec3 pos;
in layout(location=1) vec3 colour_in;

out vec3 colour_vert;

void main()
{
    // homogeneous coordinate
    gl_Position = vec4(pos, 1.0); 
    colour_vert = colour_in;
}