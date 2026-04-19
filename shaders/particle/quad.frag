#version 430 core

in vec4 pColor;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
   //fragColor = vec4(1.0, 0.0, 0.0, 1.0);

   //fragColor = pColor;
   fragColor = texture(tex, uv) * pColor;
   //fragColor = texture(tex, uv);
}

