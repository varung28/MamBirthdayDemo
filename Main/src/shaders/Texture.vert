#version 460 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vTexcoord;

layout(location = 0) out vec2 out_texcoord;

layout(binding = 0) uniform mvpMatrix 
{ 
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} uMVP;

void main(void)
{
    // Code
    gl_Position = uMVP.projectionMatrix * uMVP.viewMatrix * uMVP.modelMatrix * vPosition;
    out_texcoord = vTexcoord;
}
