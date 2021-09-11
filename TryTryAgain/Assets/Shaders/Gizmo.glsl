#ifdef VERTEX_SHADER

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;

void main()
{
    gl_Position = projection * view * model * aPos;
	Color = aColor;
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;

in vec4 Color;

void main()
{
    FragColor = Color;
} 

#endif

