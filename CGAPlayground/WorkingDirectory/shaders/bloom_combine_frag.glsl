#version 330 core
out vec4 color;

uniform sampler2D scene;
uniform sampler2D bloom;

void main() {
	vec2 texSize = textureSize(scene, 0);
	vec2 texPos = gl_FragCoord.xy / texSize.xy;
	vec4 bloomTexel = texture(bloom, texPos);
	color = texture(scene, texPos);
	color = clamp((bloomTexel + color) - (bloomTexel * color), 0.0, 1.0);
	//color += bloomTexel;
}