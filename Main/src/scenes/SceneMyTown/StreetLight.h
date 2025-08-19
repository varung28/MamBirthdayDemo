#ifndef STREET_LIGHT_H
#define STREET_LIGHT_H

#include "../../helper/Common.h"
#include "../../helper/ShaderModule.h"

class StreetLight
{
    private:
        static const int LIGHT_COUNT = 4;

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
        } LightData;

        VkDescriptorSet vkDescriptorSet_Lights = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout_Lights = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool_Lights = VK_NULL_HANDLE;
        VkPipeline vkPipeline_Lights = VK_NULL_HANDLE;
        VkPipelineLayout vkPipelineLayout_Lights = VK_NULL_HANDLE;

        LightData lightData[2]; // Middle Bar = 0, Light Lamp = 1
        UniformData uniformData;

        PushData lightsModelData[LIGHT_COUNT];

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
        StreetLight();
        ~StreetLight();

        VkResult initialize();
        void update();
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
        VkResult resize(int width, int height);
};


#endif  // STREET_LIGHT_H
