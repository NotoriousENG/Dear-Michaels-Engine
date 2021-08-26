#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 diffuse;
uniform float shininess;
uniform float metalic;

void main()
{    
	vec3 sp = diffuse;
	
    FragColor = texture(texture_diffuse1, TexCoords) + vec4(diffuse,shininess + metalic);
}