#version 330 core
out vec4 color;

uniform sampler2D scene;
uniform sampler2D glow;

void main() {
	vec2 texSize = textureSize(scene, 0);
	vec2 texPos = gl_FragCoord.xy / texSize.xy;
	color = texture(scene, texPos);
	vec4 glowing = texture(glow, texPos);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
	color *= brightness;
	
	if(length(glowing) > 0) {
		color = glowing; 
	}
}