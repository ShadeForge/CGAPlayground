#version 330 core

out vec4 color;

in vec3 CameraDir;
in vec3 NormalPlane;
in vec3 LightDir;
in vec4 fragPosLight;

uniform sampler2D depthMap;

uniform sampler2D top;
uniform sampler2D side;
uniform vec3 lightPos;

in vec3 Normal;
in vec2 TexCoords;

void main() {
	if(Normal.y > 0) {
		color = texture(top, TexCoords);
	} else {
		color = texture(side, TexCoords);
	}
	vec3 V = normalize(CameraDir);
    vec3 N = normalize(NormalPlane);
	vec3 L = normalize(LightDir);

	//color.xyz *= max(0.0, dot(N,L));
	
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = max(0.002 * (1.0 - dot(N, L)), 0.0005);
    float shadow = currentDepth + bias > closestDepth  ? 1.0 : 0.0;
	if(dot(normalize(lightPos), vec3(0, 1, 0)) < 0)
		shadow = 1;
	color.xyz *= 1 - shadow * 0.85;
}