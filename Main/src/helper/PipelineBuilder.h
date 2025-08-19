
#pragma once

#include "Common.h"
#include <vector>

class VulkanPipelineBuilder
{
public:
    std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

    VkPipelineVertexInputStateCreateInfo mVertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo mInputAssembly;
    VkViewport mViewport;
    VkRect2D mScissor;

    VkPipelineRasterizationStateCreateInfo mRasterizer;
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
    VkPipelineMultisampleStateCreateInfo mMultisampling;
    VkPipelineLayout mPipelineLayout;
    VkPipelineDepthStencilStateCreateInfo mDepthStencil;

    VkPipeline BuildPipeline(VkDevice device, VkRenderPass pass, const bool enableBlending = false);
};
