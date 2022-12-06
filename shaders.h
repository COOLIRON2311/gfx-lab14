#pragma once
const char* VertexShaderSource = R"(
#version 330 core
in vec3 coord;
in vec2 uv;
out vec2 texcoord;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(coord / 2, 1.0);
	texcoord = uv;
})";

const char* FragShaderSource = R"(
#version 330 core
in vec2 texcoord;
uniform sampler2D tex;

void main() {
	gl_FragColor = texture(tex, texcoord);
})";