#ifndef ROAD_H
#define ROAD_H

#include "../../helper/Common.h"
#include "../../helper/ShaderModule.h"

class Road
{
    private:
        static const int ROAD_GEOMETRY_COUNT = 3;

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
        } RoadData;

        VkDescriptorSet vkDescriptorSet_Road = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout_Road = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool_Road = VK_NULL_HANDLE;
        VkPipeline vkPipeline_Road = VK_NULL_HANDLE;
        VkPipelineLayout vkPipelineLayout_Road = VK_NULL_HANDLE;

        UniformData uniformData;

        RoadData roadObject[ROAD_GEOMETRY_COUNT];  // Border = 0, Main Road = 2, Zebra Crossing = 3
        PushData roadModelData[ROAD_GEOMETRY_COUNT];

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
        Road();
        ~Road();

        VkResult initialize();
        void update();
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
        VkResult resize(int width, int height);
};


#endif  // ROAD_H
