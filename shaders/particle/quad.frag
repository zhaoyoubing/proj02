#version 430 core

in vec4 vColor;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
   //fragColor = vec4(1.0, 0.0, 0.0, 1.0);

   //fragColor = vColor;
   fragColor = texture(tex, uv) * vColor;
   //fragColor = texture(tex, uv);
}

