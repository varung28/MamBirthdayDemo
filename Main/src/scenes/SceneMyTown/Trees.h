#ifndef TREES_H
#define TREES_H

#include "../../helper/Common.h"
#include "../../helper/ShaderModule.h"

class Trees
{
    private:
        const int TREES_COUNT = 1;
        const int TREES_DRAW_VERTEX_COUNT = 81;

        typedef struct
        {
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        } VertexData;

        typedef struct
        {
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatix;
        } VP_UniformData;

        typedef struct
        {
            glm::mat4 modelMatrix;
        } PushData;

        typedef struct
        {
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        } UniformData;

        typedef struct
        {
            VertexData vertexData_position;
            VertexData vertexData_color;
            PushData modelData;
        } TreeData;

        VkDescriptorSet vkDescriptorSet_Trees = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout_Trees = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool_Trees = VK_NULL_HANDLE;
        VkPipeline vkPipeline_Trees = VK_NULL_HANDLE;
        VkPipelineLayout vkPipelineLayout_Trees = VK_NULL_HANDLE;

        UniformData uniformData;
        TreeData trees[7];

        VkShaderModule vkShaderModule_vertex = VK_NULL_HANDLE;
        VkShaderModule vkShaderModule_fragment = VK_NULL_HANDLE;

    private:
        VkResult __createVertexBuffer(int index, float* position, int positionSize, float* color, int colorSize);
        VkResult createVertexBuffer();
        VkResult createUniformBuffer();
        VkResult createDescriptorSetLayout();
        VkResult createPipelineLayout();
        VkResult createDescriptorPool();
        VkResult createDescriptorSet();
        VkResult createPipeline();
        VkResult updateUniformBuffer();

    public:
        Trees();
        ~Trees();

        VkResult initialize();
        void update();
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
        VkResult resize(int width, int height);
};


#endif  // TREES_H
