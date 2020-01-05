#version 330 core

out vec4 color;

in vec3 CameraDir;
in vec3 NormalPlane;
in vec3 LightDir;
in vec4 fragPosLight;

uniform sampler2D depthMap;
uniform vec3 lightPos;

uniform vec2 repeater;
uniform float grassRange;
uniform float overlapRange;
uniform sampler2D stone;
uniform sampler2D grass;

in vec2 planePos;

void main() {
	vec2 computeVec = planePos;
	vec2 pPos = planePos * repeater + vec2(0.5);
	if(computeVec.x < 0)
		computeVec.x *= -1;
	if(computeVec.y < 0)
		computeVec.y *= -1;
	if(computeVec.x > grassRange || computeVec.y > grassRange) {
		if(computeVec.y > overlapRange || computeVec.x > overlapRange) {
			color = texture(stone, pPos);	
		} else {		
			float strength = max(computeVec.x - grassRange, computeVec.y - grassRange) / (overlapRange - grassRange);
			color = texture(grass, pPos) * (1-strength) + texture(stone, pPos) * strength;
		}
	} else {
		color = texture(grass, pPos);
	}
	vec3 V = normalize(CameraDir);
    vec3 N = normalize(NormalPlane);
	vec3 L = normalize(LightDir);

	//color.xyz *= max(0.0, dot(N,L));
	
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = max(0.0005 * (1.0 - dot(N, L)), 0.0005);
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	if(dot(normalize(lightPos), vec3(0, 1, 0)) < 0)
		shadow = 1;
	color.xyz *= 1 - shadow * 0.85;
}