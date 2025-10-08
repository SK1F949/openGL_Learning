#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3 u_Pos;
uniform mat4 u_MVP;

void main()
{   
    vec3 pos = u_Pos + aPos;
    gl_Position = u_MVP * vec4(pos, 1.0);
    TexCoord = aTexCoord;
}