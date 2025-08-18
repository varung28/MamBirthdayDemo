#pragma once

#include "../../helper/Common.h"
#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"

class ScenePradnya
{
public:
    struct VertexData
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkDeviceMemory;
    };

    struct VertexData vertexData_position;
    struct VertexData vertexData_color;

    VkDeviceSize colorVertexBuffer_AllocationSize;

    // (Ortho Change 1(a)) : Uniform-related Declarations
    struct MyUniformData
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

    struct UniformData
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkDeviceMemory;
    };

    struct UniformData uniformData;

    // "Shaders"-related Global Variables
    // (Shaders) Step 1 : Write and build the shaders
    // (Shaders) Step 2 : Globally declare 2 shader module object variables of type 'VkShaderModule' to hold Vulkan-compatible Vertex Shader Module and Fragment Shader Module
    VkShaderModule vkShaderModule_vertex_shader = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_fragment_shader = VK_NULL_HANDLE;

    // "Descriptor Set Layout"-related Global Variables
    // (Descriptor Set Layout) Step 1 : Globally declare Vulkan Object of type 'VkDescriptorSetLayout' and initialize it to VK_NULL_HANDLE
    VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    // "Pipeline Layout"-related Global Variables
    // (Pipeline Set Layout) Step 1 : Globally declare Vulkan Object of type 'VkPipelineLayout' and initialize it to VK_NULL_HANDLE
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    // (Ortho Change 1(b)) : Descriptor Pool
    VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;

    // (Ortho Change 1(c)) : Descriptor Set
    VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;

    // "Pipeline"-related Global Variables
    // (Pipeline) Step 1 : Declare following 3 variables globally
    VkViewport vkViewport;
    VkRect2D vkRect2D_scissor;
    VkPipeline vkPipeline = VK_NULL_HANDLE;

    VkResult initialize(void);
    VkResult resize(int width, int height);
    VkResult update(void);
    VkResult update_sky(void);

    void uninitialize(void);
    // (Vertex Buffer) Step 2(c) : Defining createVertexBuffer() at this specific position
    VkResult createVertexBuffer(void);
    // (Ortho Change 7(a)) : Defining createUniformBuffer()
    VkResult createUniformBuffer(void);
    // (Ortho Change 7(b)) : Defining updateUniformBuffer()
    VkResult updateUniformBuffer(bool bSky);
    VkResult updateColorVertexBuffer(void);
    VkResult updateColorVertexBuffer_sky(void);
    // (Shaders) Step 5 : Defining createShaders() at this specific position.
    VkResult createShaders(void);
    VkResult createDescriptorSetLayout(void);
    VkResult createPipelineLayout(void);
    // (Ortho Change 7(c)) : Defining createDescriptorPool()
    VkResult createDescriptorPool(void);
    // (Ortho Change 7(d)) : Defining createDescriptorSet()
    VkResult createDescriptorSet(void);
    // (Pipeline) Step 2(c) : Defining createPipeline() at this specific position.
    VkResult createPipeline(void);
    VkResult buildCommandBuffers(VkCommandBuffer &commandBuffer);
};
