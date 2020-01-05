#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMatrix;
uniform float time;
uniform vec2 windDir;

void main() {
	vec3 pos = position;
	vec4 localPos = modelMatrix * vec4(pos, 1.0);
	float flex = (pos.y + 1); 
	pos.x += sin(time + sin(localPos.x * 4) * 3.14) * 0.25 * flex;
	pos.z += cos(time + cos(localPos.z * 4) * 3.14) * 0.25 * flex;
	pos.xz += windDir * flex;
	//pos.xz = normalize(pos.xz) * 2;
	gl_Position = projection * view * modelMatrix * vec4(pos, 1.0);
	TexCoords = texCoord;
}