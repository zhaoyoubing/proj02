#version 430 core

layout (location = 0) in vec3 aPos;   // Point position
//layout (location = 1) in vec3 aColor; // Optional per-point colour

// camera view projection matrix.
uniform mat4 view;
uniform mat4 proj;

out vec3 vColor;

void main()
{
    //gl_Position = proj * view * vec4(aPos, 1.0);
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);

    // Set the size of the point (in pixels)
    gl_PointSize = 10.0;

    vColor = vec3(1.0, 0.0, 0.0);
}