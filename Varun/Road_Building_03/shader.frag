#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 fragcolor;
layout (location = 0) in vec4 out_color;

void main(void)
{
	fragcolor = out_color;
}
