#version 330 core
in vec2 uv;
uniform sampler2D tex;

vec3 Texture()
{
	return texture(tex, uv);
}

void main() {
	vec3 outp = Texture();
	gl_FragColor = vec4(outp, 1.0);
}
