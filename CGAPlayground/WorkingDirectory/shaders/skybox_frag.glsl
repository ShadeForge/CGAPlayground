#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform mat4 modelMatrix;
uniform float horizonOffset;
uniform vec3 upperHorizon;
uniform vec3 lowerHorizon;

void main()
{    
    color = texture(skybox, TexCoords);
	if(color.a < 1) {
		vec3 coords = (modelMatrix * vec4(TexCoords, 1.0)).xyz;
		float strength = dot(normalize(coords), vec3(0, 1, 0)) + horizonOffset;
		color = vec4(lowerHorizon * (1 - strength) + upperHorizon * strength, 1);
	}
}