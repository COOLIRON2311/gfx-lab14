#version 330 core
in vec3 coord;
in vec2 texcoord;
out vec2 uv;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(coord / 2, 1.0);
	uv = texcoord;
}
