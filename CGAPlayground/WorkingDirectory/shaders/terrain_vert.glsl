#version 330 core
layout (location = 0) in vec3 position;

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

out vec2 planePos;

void main() {
	gl_Position = projection * view * modelMatrix * vec4(position, 1.0);
	planePos = position.xz;
	
	vec3 normal = vec3(0, 1, 0);
	
    vec4 p = vec4(position, 1.0);
    vec4 n = vec4(normal, 1.0);
    vec4 P = (view * modelMatrix * p);

    NormalPlane = (transpose(inverse(view * modelMatrix)) * n).xyz;
    CameraDir = -P.xyz;
	
	vec4 lp = vec4(lightPos, 1.0);
	LightDir = ((view * lp) - P).xyz;
	fragPosLight = lightProjection * lightView * modelMatrix * vec4(position,1.0);
}