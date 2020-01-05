#version 330 core
out vec4 color;

uniform sampler2D depth;
uniform sampler2D shadowMap;
uniform sampler2D scene;
uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 projection;
uniform mat4 view;

void main() {
	vec2 texSize = textureSize(scene, 0);
	vec2 texPos = gl_FragCoord.xy / texSize.xy;
	color = texture(scene, texPos);
	float z = texture(depth, texPos).r;
	
	//vec3 ndc_cam = vec3(texPos, z);
    //vec4 clip_cam = vec4(ndc_cam,1);
    //vec4 clip_light = lightProjection * lightView * inverse(view) * inverse(projection) * clip_cam;
    //vec3 ndc_light = clip_light.xyz / clip_light.w;
    //vec3 texco = (ndc_light + 1) / 2;
	
	vec4 viewPos = inverse(view) * vec4(texPos.x, texPos.y, z, 1);
	vec3 worldPos = viewPos.xyz / viewPos.w;
	vec4 lightViewPos = lightView * vec4(worldPos, 1);
	vec3 shadowTexPos = lightViewPos.xyz / lightViewPos.w;
	
    float closestDepth = texture(depth, shadowTexPos.xy).r; 
    float currentDepth = shadowTexPos.z;
	float bias = 0.002;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	if(shadowTexPos.z <= 1.0)
		color.xyz *= 1 - shadow;
		
	color.xyz *= 1 - texture(depth, texPos).r;
}