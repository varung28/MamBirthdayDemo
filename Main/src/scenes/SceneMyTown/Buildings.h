#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "../../helper/Common.h"
#include "../../helper/ShaderModule.h"

class Buildings
{
    private:
        const int BUILDING_COUNT = 5;
        const int BUILDING_GEOMETRY_COUNT = 7;
        
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
        } BuildingData;

        VkDescriptorSet vkDescriptorSet_Buildings = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout_Buildings = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool_Buildings = VK_NULL_HANDLE;
        VkPipeline vkPipeline_Buildings = VK_NULL_HANDLE;
        VkPipelineLayout vkPipelineLayout_Buildings = VK_NULL_HANDLE;

        UniformData uniformData;
        BuildingData buildings[7];

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
        Buildings();
        ~Buildings();

        VkResult initialize();
        void update();
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
        VkResult resize(int width, int height);
};


#endif  // BUILDINGS_H
