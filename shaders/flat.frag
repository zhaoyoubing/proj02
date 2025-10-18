#version 410

out vec4 colour_out;

// for lighting
in vec3 fragPos;
in flat vec3 normal;

uniform vec3 lightPos;
//uniform vec3 viewPos;

void main()
{

    // manually set the vertex colour
    vec3 color = vec3(1.0, 0.0, 0.0);

    // 1. ambient
    vec3 ambient = 0.05 * color;

    // 2. diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // The final combined colour
    colour_out = vec4(ambient + diffuse, 1.0);
}