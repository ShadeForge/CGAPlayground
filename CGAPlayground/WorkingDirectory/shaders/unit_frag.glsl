#version 330 core
out vec4 color;
out vec4 glow;

in vec3 CameraDir;
in vec3 NormalPlane;
in vec3 LightDir;
in vec4 fragPosLight;

uniform sampler2D depthMap;
uniform vec3 lightPos;
uniform bool glowing;

uniform vec3 unitColor;

void main() {
	color = vec4(unitColor, 1.0);
	if(glowing)
		glow = color;
	else
		glow = vec4(0);
		
	vec3 V = normalize(CameraDir);
    vec3 N = normalize(NormalPlane);
	vec3 L = normalize(LightDir);

	//color.xyz *= max(0.0, dot(N,L));
	
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = max(0.0005 * (1.0 - dot(N, L)), 0.0005);
    float shadow = currentDepth - 0.0005  > closestDepth  ? 1.0 : 0.0;
	if(dot(normalize(lightPos), vec3(0, 1, 0)) < 0)
		shadow = 1;
	if(!glowing)
		color.xyz *= 1 - shadow * 0.85;
}