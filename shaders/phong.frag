#version 410

out vec4 colour_out;
// in vec3 colour_vert;

// for lighting
in vec3 fragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // RGBA
    //colour_out = vec4(1.0, 0.0, 0.0, 1.0);
    //colour_out = vec4(colour_vert, 1.0);

    // manually set the vertex colour
    vec3 color = vec3(1.0, 0.0, 0.0);

    // 1. ambient
    vec3 ambient = 0.05 * color;

    // 2. diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // 3. specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    //vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, reflectDir), 0.0), 2.0);

    // assuming bright white light colour
    vec3 specular = vec3(0.3) * spec;

    // The final combined colour
    colour_out = vec4(ambient + diffuse + specular, 1.0);
}