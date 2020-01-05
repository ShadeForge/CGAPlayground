#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

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

uniform float time;
uniform vec2 windDir;

void main() {
	vec3 pos = position;
	vec4 localPos = modelMatrix * vec4(pos, 1.0);
	float flex = (pos.y + 1); 
	pos.x += sin(time + sin(localPos.x * 4) * 3.14) * 0.25 * flex;
	pos.z += cos(time + cos(localPos.z * 4) * 3.14) * 0.25 * flex;
	pos.xz += windDir * flex;
	gl_Position = projection * view * modelMatrix * vec4(pos, 1.0);
	TexCoords = texCoord;
	
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