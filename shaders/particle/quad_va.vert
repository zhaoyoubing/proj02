#version 430 core

layout(location = 0) in vec3 aPos;      // quad vertex
layout(location = 1) in vec4 partPos;   // per-particle position
layout(location = 2) in vec4 color;   // per-particle position

uniform mat4 view;
uniform mat4 proj;

out vec4 vColor;
out vec2 uv;

void main() {

    vec3 worldPos = partPos.xyz + aPos * 0.2;
    gl_Position = proj * view * vec4(worldPos, 1.0);

    vColor = color;

    if (gl_VertexID % 6 == 0) uv = vec2(0,0);
    if (gl_VertexID % 6 == 1) uv = vec2(1,0);
    if (gl_VertexID % 6 == 2) uv = vec2(1,1);
    if (gl_VertexID % 6 == 3) uv = vec2(0,0);
    if (gl_VertexID % 6 == 4) uv = vec2(1,1);
    if (gl_VertexID % 6 == 5) uv = vec2(0,1);
}
