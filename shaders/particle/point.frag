#version 430 core

in vec4 vColor;
out vec4 FragColor;

//uniform sampler2D tex;

void main()
{
    FragColor = vColor;
}