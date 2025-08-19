#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

layout(location = 0) out vec4 out_Color;

layout(binding = 0) uniform MVPMatrix {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} uMPV;

void main() {
    out_Color = vColor;
    gl_Position = uMPV.projectionMatrix * uMPV.viewMatrix * uMPV.modelMatrix * vPosition;
}
