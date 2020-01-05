#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec2 TexCoords;

out vec3 CameraDir;
out vec3 NormalPlane;
out vec3 LightDir;
out vec4 fragPosLight;

uniform mat4 modelMatrix;
uniform mat4 projection;
uniform mat4 view;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform vec3 lightPos;

uniform vec3 repeater;

void main() {
	gl_Position = projection * view * modelMatrix * vec4(position, 1.0);
	
	if(normal.y == 1) {
		TexCoords = position.xz;
		TexCoords.x *= repeater.x;
	} if (normal.x == 1) {
		TexCoords = position.zy;
		TexCoords.x *= repeater.y;
	} else {
		TexCoords = position.xy;
		TexCoords.x *= repeater.z;
	}
	TexCoords += vec2(0.5);
	Normal = normal;
	
    vec4 p = vec4(position, 1.0);
    vec4 n = vec4(normal, 1.0);
    vec4 P = (view * modelMatrix * p);

    NormalPlane = (transpose(inverse(view * modelMatrix)) * n).xyz;
    CameraDir = -P.xyz;
	
	vec4 lp = vec4(lightPos, 1.0);
	LightDir = ((view * lp) - P).xyz;
	fragPosLight = lightProjection * lightView * modelMatrix * vec4(position,1.0);
}