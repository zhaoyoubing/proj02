#version 410

in layout(location=0) vec3 pos;
in layout(location=1) vec3 aNormal;

// transformation in the world and camera space
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec3 colour_vert;
out vec3 normal;
out vec3 fragPos;

void main()
{
    // homogeneous coordinate
    gl_Position = projection * view * model * vec4(pos, 1.0); 
    
    // colour_vert = colour_in;
    fragPos = vec3(model * vec4(pos, 1.0));

    mat3 normalMatrix = mat3(model);
    // only correct for rigid body transforms
    normal = normalMatrix * aNormal;
}