#ifndef STARS_H
#define STARS_H

#include "../../helper/Common.h"
#include "../../helper/ShaderModule.h"

class Stars
{
private:
    static const int STARS_COUNT = 10;
    const int STAR_DRAW_VERTEX_COUNT = 6;

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

    VkDescriptorSet vkDescriptorSet_Stars= VK_NULL_HANDLE;
    VkDescriptorSetLayout vkDescriptorSetLayout_Stars= VK_NULL_HANDLE;
    VkDescriptorPool vkDescriptorPool_Stars= VK_NULL_HANDLE;
    VkPipeline vkPipeline_Stars= VK_NULL_HANDLE;
    VkPipelineLayout vkPipelineLayout_Stars= VK_NULL_HANDLE;

    VertexData vertexData_position_star;
    VertexData vertexData_color_star;
    UniformData uniformData;

    PushData starsModelData[STARS_COUNT];

    VkShaderModule vkShaderModule_vertex = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_fragment = VK_NULL_HANDLE;

private:
    VkResult createVertexBuffer();
    VkResult createUniformBuffer();
    VkResult createDescriptorSetLayout();
    VkResult createPipelineLayout();
    VkResult createDescriptorPool();
    VkResult createDescriptorSet();
    VkResult createPipeline();
    VkResult updateUniformBuffer();

public:
    Stars();
    ~Stars();

    VkResult initialize();
    void update();
    void buildCommandBuffers(VkCommandBuffer& commandBuffer);
    VkResult resize(int width, int height);
};


#endif  // STARS_H
