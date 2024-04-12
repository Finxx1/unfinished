#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texcoord;

out vec2 texcoord2;

uniform vec4 bounds;

void main() {
	vec2 pos2 = texcoord;
	if (pos.x > 0) {
		pos2.x = bounds.z;
	}
	if (pos.y > 0) {
		pos2.y = bounds.w;
	}
	pos2 += bounds.xy;
	pos2 *= 2.0;
	pos2 -= 1.0;
	gl_Position = vec4(pos2, 0.0, 1.0);

	texcoord2 = texcoord;
}

