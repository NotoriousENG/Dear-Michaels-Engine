#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;

in vec2 TexCoords;
uniform bool useTexture;
uniform sampler2D texture_diffuse1;
uniform vec4 Albedo;

void main()
{    
    if (!useTexture)
    {
        FragColor = Albedo;
    }

    else
    {
        FragColor = texture(texture_diffuse1, TexCoords) * Albedo;
    }
}

#endif

