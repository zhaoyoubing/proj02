#version 410

out vec4 colour_out;
in vec3 colour_in;

void main()
{
    // RGBA
    colour_out = vec4(colour_in, 1.0);
}