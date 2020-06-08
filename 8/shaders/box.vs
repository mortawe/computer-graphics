#version 330 core
layout (location = 3) in vec3 aPos;
layout (location = 4) in vec3 color;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Color = color;
    gl_Position = projection * view * vec4(aPos, 1.0);
}