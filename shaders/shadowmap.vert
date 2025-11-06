// shadowmap.vert
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

out vec4 fragPosLightSpace;

void main()
{   
    gl_Position = projection * view * model * vec4(aPos, 1.0); 

    fragPos = vec3(model * vec4(aPos, 1.0));
    
    mat3 normalMatrix = mat3(model);
    // !! only correct for rigid body transforms
    normal = normalMatrix * aNormal;
    // the correct normal transform
    // normal = transpose(inverse(mat3(model))) * aNormal;

    texCoord = aTexCoord;
    
    fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
}