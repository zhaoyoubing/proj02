// LabA08 Normal Map
// normalmap.vert
#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 aTexCoord;

// added for LabA08 Normal Map
in layout(location=3) vec3 aTangent;
in layout(location=4) vec3 aBitangent;

// M V P matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// moved from the fragment shader for LabA08 Normal Map
uniform vec3 lightPos;
uniform vec3 viewPos;

//out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

// added for LabA08 Normal Map
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;


void main()
{
    // homogeneous coordinate
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    
    fragPos = vec3(model * vec4(aPos, 1.0));

    // commented out for LabA08 
    // mat3 normalMatrix = mat3(model);
    // normal = normalMatrix * aNormal;

    texCoord = aTexCoord;

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    mat3 TBN = transpose(mat3(T, B, N));

    tangentLightPos = TBN * lightPos;
    tangentViewPos  = TBN * viewPos;
    tangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));
}