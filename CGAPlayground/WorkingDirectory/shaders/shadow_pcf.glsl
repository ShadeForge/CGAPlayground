#version 330 core
out vec4 color;

uniform sampler2D shadowMap;
uniform int kernelSize;

void main() {
	vec2 texSize = textureSize(shadowMap, 0);
	vec2 texPos = gl_FragCoord.xy / texSize;
    vec2 tex_offset = 1.0 / texSize;
	color = vec4(0);
	
	for(int x = -int(kernelSize/2); x < int(kernelSize/2) + 1; x++) {
		for(int y = -int(kernelSize/2); y < int(kernelSize/2) + 1; y++) {
			color += texture(shadowMap, texPos + vec2(x, y) * tex_offset);
		}
	}
	color /= kernelSize * kernelSize;
	color.a = 1;
}