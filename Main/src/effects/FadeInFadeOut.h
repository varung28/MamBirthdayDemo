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

    FadeInFadeOut(const FadeInFadeOut &) = delete;
    FadeInFadeOut operator=(const FadeInFadeOut &) = delete;

    void display();
    void update();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void createPipeline();
    VkResult createDescriptorSet();
    void onResize(const int width, const int height);
    void update(float _alpha);

    float alpha{0.0f};

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
    };

    static struct UniformData uniformData;

    bool isFadingIn{false};

    static Quad *quad;
    static VkShaderModule vertexShaderModule;
    static VkShaderModule fragmentShaderModule;
    static VkPipeline vkPipeline;
    static VkDescriptorSetLayout vkDescriptorSetLayout;
    static VkDescriptorSet vkDescriptorSet;
    static VulkanPipelineBuilder *pipelineBuilder;
    static VkPipelineLayout vkPipelineLayout;
};
