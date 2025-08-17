#ifndef TEXTUREDQUAD_H
#define TEXTUREDQUAD_H

#include "Common.h"
#include "Geometry.h"
#include "ShaderModule.h"
#include "Texture.h"

class TexturedQuad
{
    private:
        VkShaderModule vkShaderModule_vertex = VK_NULL_HANDLE;
        VkShaderModule vkShaderModule_fragment = VK_NULL_HANDLE;
        Texture* texture;
        Quad* quad;

        typedef struct
        {
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        } UniformData;

        typedef struct 
        {
            glm::mat4 modelMatrix;
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
        } MVP_UniformData;

        UniformData uniformData;

        VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
        VkPipeline vkPipeline = VK_NULL_HANDLE;

        VkResult createUniformBuffer();
        VkResult createDescriptorSetLayout();
        VkResult createPipelineLayout();
        VkResult createDescriptorPool();
        VkResult createDescriptorSet();
        VkResult createPipeline();


    public:
        TexturedQuad();
        ~TexturedQuad();

        VkResult initialize(std::string filePath);
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
        VkResult updateUniformBuffer();
};

#endif // TEXTUREDQUAD_H
