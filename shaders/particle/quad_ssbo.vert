#version 430 core

layout(location = 0) in vec3 aPos;      // quad vertex
//layout(location = 1) in vec4 partPos;   // per-particle position
//layout(location = 2) in vec4 color;   // per-particle position

struct Particle {
    vec4 pos;
    vec4 velocity;
    vec4 color;
    float life;
    float maxLife;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform mat4 view;
uniform mat4 proj;

out vec4 pColor;
out vec2 uv;

void main() {

    // 🔑 Get particle using instance ID
    Particle p = particles[gl_InstanceID];

    vec3 worldPos = p.pos.xyz + aPos * 0.2;
    gl_Position = proj * view * vec4(worldPos, 1.0);

    pColor = p.color;

    if (gl_VertexID % 6 == 0) uv = vec2(0,0);
    if (gl_VertexID % 6 == 1) uv = vec2(1,0);
    if (gl_VertexID % 6 == 2) uv = vec2(1,1);
    if (gl_VertexID % 6 == 3) uv = vec2(0,0);
    if (gl_VertexID % 6 == 4) uv = vec2(1,1);
    if (gl_VertexID % 6 == 5) uv = vec2(0,1);
}
