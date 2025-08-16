#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 out_Color;

layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(0.98, 0.3, 0.01, 1.0);
}
