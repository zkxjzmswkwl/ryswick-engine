#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * aPos;
    v_TexCoord = texCoord;
}
