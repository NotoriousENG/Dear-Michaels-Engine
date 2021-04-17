#version 330 core
out vec4 FragColor;

uniform vec4 PickingColor;

void main()
{
    FragColor = PickingColor;
} 