#version 460 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 out_color;

layout(location = 0) out vec4 FragColor;

void main(void)
{
    // Code
    FragColor = out_color;
}
