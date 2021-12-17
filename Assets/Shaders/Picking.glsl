#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;

uniform mat4 MVP;

void main()
{
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(aPos,1);
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 FragColor;

uniform vec4 PickingColor;

void main()
{
    FragColor = PickingColor;
} 

#endif

