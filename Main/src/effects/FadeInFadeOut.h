#pragma once

#include "../helper/Common.h"
#include "../helper/Geometry.h"
#include "../helper/ShaderModule.h"
#include "../helper/PipelineBuilder.h"

class FadeInFadeOut
{
public:
    FadeInFadeOut();
    ~FadeInFadeOut();

    FadeInFadeOut(const FadeInFadeOut&) = delete;
    FadeInFadeOut operator=(const FadeInFadeOut&) = delete;

    void display();
    void update();
    void initialCommandBuffer(VkCommandBuffer& commandBuffer);
    void createPipeline();
    VkResult createDescriptorSet();
    void onResize(const int width, const int height);

private:
    VkResult updateUniformBuffer();
    VkResult createUniformBuffer();
    VkResult createDescriptorSetLayout();
    VkResult createPipelineLayout();


    struct MyUniformData
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        float blendFactor{0.0f};
    };

    struct UniformData
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkDeviceMemory;
    } uniformData;

    float alpha{0.0f};
    bool isFadingIn{false};

    Quad *quad {nullptr};
    VkShaderModule vertexShaderModule {VK_NULL_HANDLE};
    VkShaderModule fragmentShaderModule {VK_NULL_HANDLE};
    VkPipeline vkPipeline{VK_NULL_HANDLE};
    VkDescriptorSetLayout vkDescriptorSetLayout {VK_NULL_HANDLE};
    VkDescriptorSet vkDescriptorSet{VK_NULL_HANDLE};
    VulkanPipelineBuilder *pipelineBuilder{nullptr};
    VkPipelineLayout vkPipelineLayout {VK_NULL_HANDLE};
};
