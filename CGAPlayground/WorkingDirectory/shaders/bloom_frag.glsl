#version 330 core
out vec4 color;

uniform int orientation;
uniform vec2 targetSize;
uniform sampler2D lumination;

const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
	vec2 texPos = gl_FragCoord.xy / targetSize;
    vec2 tex_offset = 1.0 / targetSize;
	color = texture(lumination, texPos) * weight[0];

    if(orientation == 0)
    {
        for(int i = 1; i < 5; ++i)
        {
            color += texture(lumination, texPos + vec2(tex_offset.x * i, 0.0)) * weight[i];
            color += texture(lumination, texPos - vec2(tex_offset.x * i, 0.0)) * weight[i];
        }
    } else {
        for(int i = 1; i < 5; ++i)
        {
            color += texture(lumination, texPos + vec2(0.0, tex_offset.y * i)) * weight[i];
            color += texture(lumination, texPos - vec2(0.0, tex_offset.y * i)) * weight[i];
        }
    }
}