#include "PipelineBuilder.h"

VkPipeline VulkanPipelineBuilder::BuildPipeline(VkDevice device, VkRenderPass pass)
{
    VkResult vkResult = VK_SUCCESS;

    VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo;
    memset((void *)&vkPipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

    vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vkPipelineInputAssemblyStateCreateInfo.flags = 0;
    vkPipelineInputAssemblyStateCreateInfo.pNext = NULL;
    vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    // RISTERRISE STATE
    VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo;
    memset((void *)&vkPipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

    vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    vkPipelineRasterizationStateCreateInfo.pNext = NULL;
    vkPipelineRasterizationStateCreateInfo.flags = 0;
    vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    vkPipelineRasterizationStateCreateInfo.cullMode = NULL;
    vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

    // COLOR BLEND STATE
    VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState_array[1];
    memset((void *)vkPipelineColorBlendAttachmentState_array, 0, sizeof(VkPipelineColorBlendAttachmentState) * _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array));

    vkPipelineColorBlendAttachmentState_array[0].blendEnable = VK_FALSE;
    vkPipelineColorBlendAttachmentState_array[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo;
    memset((void *)&vkPipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

    vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vkPipelineColorBlendStateCreateInfo.pNext = NULL;
    vkPipelineColorBlendStateCreateInfo.flags = 0;
    vkPipelineColorBlendStateCreateInfo.attachmentCount = _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array);
    vkPipelineColorBlendStateCreateInfo.pAttachments = vkPipelineColorBlendAttachmentState_array;

    // VIEW PORT & SCISSOR STATE
    memset((void *)&vkViewport, 0, sizeof(VkViewport));

    vkViewport.x = 0;
    vkViewport.y = 0;
    vkViewport.width = (float)vkExtent2D_swapchain.width;
    vkViewport.height = (float)vkExtent2D_swapchain.height;
    vkViewport.minDepth = 0.0f;
    vkViewport.maxDepth = 1.0f;

    memset((void *)&vkRect2D_Scissor, 0, sizeof(VkRect2D));

    vkRect2D_Scissor.offset.x = 0;
    vkRect2D_Scissor.offset.y = 0;
    vkRect2D_Scissor.extent.width = vkExtent2D_swapchain.width;
    vkRect2D_Scissor.extent.height = vkExtent2D_swapchain.height;

    VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo;
    memset((void *)&vkPipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));

    vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vkPipelineViewportStateCreateInfo.pNext = NULL;
    vkPipelineViewportStateCreateInfo.flags = 0;
    vkPipelineViewportStateCreateInfo.viewportCount = 1; // WE CAN SPECIFY MULTIPLE VIEWPORT
    vkPipelineViewportStateCreateInfo.pViewports = &vkViewport;

    vkPipelineViewportStateCreateInfo.scissorCount = 1;
    vkPipelineViewportStateCreateInfo.pScissors = &vkRect2D_Scissor;

    // DEPTH STENCIL STATE (AS WE DONT HAVE DEPTH YET, SO WE ARE OMMITING THI STATE)
    VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo;
    memset((void *)&vkPipelineDepthStencilStateCreateInfo, 0, sizeof(VkPipelineDepthStencilStateCreateInfo));

    vkPipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    vkPipelineDepthStencilStateCreateInfo.flags = 0;
    vkPipelineDepthStencilStateCreateInfo.pNext = NULL;
    vkPipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    vkPipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    vkPipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    vkPipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    vkPipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    vkPipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    vkPipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    vkPipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    vkPipelineDepthStencilStateCreateInfo.front = vkPipelineDepthStencilStateCreateInfo.back;

    // DYAMIC STATE (WE DONT HAVE DYANMIC STATE)

    // MULTI SAMPLING STATE
    VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo;
    memset((void *)&vkPipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

    vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    vkPipelineMultisampleStateCreateInfo.pNext = NULL;
    vkPipelineMultisampleStateCreateInfo.flags = 0;
    vkPipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineViewportStateCreateInfo viewportState{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &mViewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &mScissor;

    // this->mInputAssembly = vkPipelineInputAssemblyStateCreateInfo;
    // this->mRasterizer = vkPipelineRasterizationStateCreateInfo;
    // this->colorBlendStateCreateInfo = vkPipelineColorBlendStateCreateInfo;
    // this->mDepthStencil = vkPipelineDepthStencilStateCreateInfo;
    // this->mMultisampling = vkPipelineMultisampleStateCreateInfo;

    // CREATE THE ACTUAL GRAPHICS PIPELINE
    VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo;
    memset((void *)&vkGraphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));

    vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    vkGraphicsPipelineCreateInfo.pNext = NULL;
    vkGraphicsPipelineCreateInfo.flags = 0;
    vkGraphicsPipelineCreateInfo.pVertexInputState = &mVertexInputInfo;
    vkGraphicsPipelineCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pColorBlendState = &vkPipelineColorBlendStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pViewportState = &viewportState;
    vkGraphicsPipelineCreateInfo.pDepthStencilState = &vkPipelineDepthStencilStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
    vkGraphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(mShaderStages.size());
    vkGraphicsPipelineCreateInfo.pStages = mShaderStages.data();
    vkGraphicsPipelineCreateInfo.pTessellationState = NULL;
    vkGraphicsPipelineCreateInfo.layout = mPipelineLayout;
    vkGraphicsPipelineCreateInfo.renderPass = pass;
    vkGraphicsPipelineCreateInfo.subpass = 0;
    vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    vkGraphicsPipelineCreateInfo.basePipelineIndex = 0;

    // AS PIPELINE CREATES FROM PIPELINE CACHE, SO WE CREATING PIPELINE CACHE OBJECT
    VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
    memset((void *)&vkPipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));

    vkPipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vkPipelineCacheCreateInfo.pNext = NULL;
    vkPipelineCacheCreateInfo.flags = 0;

    VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

    vkResult = vkCreatePipelineCache(vkDevice, &vkPipelineCacheCreateInfo, NULL, &vkPipelineCache);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreatePipelineCache() IS FAILED.\n", __func__);

        return VK_NULL_HANDLE;
    }
    else
    {
        fprintf(gpFile, "%s => vkCreatePipelineCache() IS SUCCEEDED.\n", __func__);
    }

    VkPipeline newPipeline;

    if (vkCreateGraphicsPipelines(device, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, nullptr, &newPipeline) != VK_SUCCESS)
    {
        // std::cout << "Failed to create pipeline\n";
        fprintf(gpFile, "%s => vkCreateGraphicsPipelines() IS FAILED.\n", __func__);
        return VK_NULL_HANDLE;
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateGraphicsPipelines() IS SUCCEEDED.\n", __func__);
    }

    if (vkPipelineCache)
    {
        vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
        vkPipelineCache = VK_NULL_HANDLE;
    }

    return newPipeline;
}
