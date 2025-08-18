#include "Buildings.h"

extern int winWidth;
extern int winHeight;

VkResult Buildings::__createVertexBuffer(int index, float* position, int positionSize, float* color, int colorSize)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;
    VkBufferCreateInfo vkBufferCreateInfo;
    VkMemoryRequirements vkMemoryRequirements;
    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    void* data = NULL;

    //! Vertex Position
    //! ---------------------------------------------------------------------------------------------------------------------------------
    //* Step - 4
    memset((void*)&buildings[index].vertexData_position, 0, sizeof(VertexData));

    //* Step - 5
    
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;   //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = positionSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &buildings[index].vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    
    //* Step - 7
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, buildings[index].vertexData_position.vkBuffer, &vkMemoryRequirements);

    //* Step - 8
    memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0;
    
    //* Step - 8.1
    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        //* Step - 8.2
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            //* Step - 8.3
            if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                //* Step - 8.4
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }

        //* Step - 8.5
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    //* Step - 9
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &buildings[index].vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, buildings[index].vertexData_position.vkBuffer, buildings[index].vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 11
    vkResult = vkMapMemory(vkDevice, buildings[index].vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 12
    memcpy(data, position, positionSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, buildings[index].vertexData_position.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    //! Vertex Color
    //! ---------------------------------------------------------------------------------------------------------------------------------
    //* Step - 4
    memset((void*)&buildings[index].vertexData_color, 0, sizeof(VertexData));

    //* Step - 5
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;   //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = colorSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &buildings[index].vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    
    //* Step - 7
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, buildings[index].vertexData_color.vkBuffer, &vkMemoryRequirements);

    //* Step - 8
    memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0;
    
    //* Step - 8.1
    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        //* Step - 8.2
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            //* Step - 8.3
            if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                //* Step - 8.4
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }

        //* Step - 8.5
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    //* Step - 9
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &buildings[index].vertexData_color.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, buildings[index].vertexData_color.vkBuffer, buildings[index].vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 11
    data = NULL;
    vkResult = vkMapMemory(vkDevice, buildings[index].vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 12
    memcpy(data, color, colorSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, buildings[index].vertexData_color.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

Buildings::Buildings()
{
    vkShaderModule_vertex = ShaderModuleHelper::LoadShaderModule("bin\\PushConstant.vert.spv");
    vkShaderModule_fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader.frag.spv"); 
}

VkResult Buildings::initialize()
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    vkResult = createVertexBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createVertexBuffer() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createVertexBuffer() Succeeded\n", __func__);

    vkResult = createUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createUniformBuffer() Succeeded\n", __func__);

    vkResult = createDescriptorSetLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createDescriptorSetLayout() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createDescriptorSetLayout() Succeeded\n", __func__);

    vkResult = createPipelineLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS : %s() => createPipelineLayout() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createPipelineLayout() Succeeded\n", __func__);

    vkResult = createDescriptorPool();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createDescriptorPool() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createDescriptorPool() Succeeded\n", __func__);

    vkResult = createDescriptorSet();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createDescriptorSet() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createDescriptorSet() Succeeded\n", __func__);

    vkResult = createPipeline();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::ERROR : %s() => createPipeline() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
    else
        fprintf(gpFile, "BUILDINGS::%s() => createPipeline() Succeeded\n", __func__);


    return vkResult;
}

VkResult Buildings::createVertexBuffer(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    float triangle_position[] = 
    {
        0.0f,   1.0f,   0.0f,
        -1.0f,  -1.0f,  0.0f,
        1.0f,   -1.0f,  0.0f  
    };

    float triangle_color[] = 
    {
        0.498f,   0.450f,   0.443f,
        0.498f,   0.450f,   0.443f,
        0.498f,   0.450f,   0.443f
    };

    float rectangle_position[] = 
    {
        1.0f,   1.0f,   0.0f,
        -1.0f,  1.0f,   0.0f,
        -1.0f,  -1.0f,  0.0f,
        -1.0f,  -1.0f,  0.0f,
        1.0f,   -1.0f,  0.0f,
        1.0f,   1.0f,   0.0f
    };

    float rectangle_5_position[] = 
    {
        0.1f,   1.0f,   0.0f,
        -1.0f,  1.0f,   0.0f,
        -1.0f,  -1.0f,  0.0f,
        -1.0f,  -1.0f,  0.0f,
        1.1f,   -1.0f,  0.0f,
        0.1f,   1.0f,   0.0f
    };

    float rectangle_1_color[] = 
    {
        0.443f,   0.450f,   0.494f,
        0.443f,   0.450f,   0.494f,
        0.443f,   0.450f,   0.494f,
        0.443f,   0.450f,   0.494f,
        0.443f,   0.450f,   0.494f,
        0.443f,   0.450f,   0.494f
    };

    float rectangle_2_color[] = 
    {
        0.396f,   0.4f,   0.541f,
        0.396f,   0.4f,   0.541f,
        0.396f,   0.4f,   0.541f,
        0.396f,   0.4f,   0.541f,
        0.396f,   0.4f,   0.541f,
        0.396f,   0.4f,   0.541f
    };

    float rectangle_3_color[] = 
    {
        0.890f,   0.835f,   0.784f,
        0.890f,   0.835f,   0.784f,
        0.890f,   0.835f,   0.784f,
        0.890f,   0.835f,   0.784f,
        0.890f,   0.835f,   0.784f,
        0.890f,   0.835f,   0.784f
    };

    float rectangle_4_color[] = 
    {
        0.647f,   0.647f,   0.647f,
        0.647f,   0.647f,   0.647f,
        0.647f,   0.647f,   0.647f,
        0.647f,   0.647f,   0.647f,
        0.647f,   0.647f,   0.647f,
        0.647f,   0.647f,   0.647f
    };

    float rectangle_5_color[] = 
    {
        0.501f,   0.501f,   0.501f,
        0.501f,   0.501f,   0.501f,
        0.501f,   0.501f,   0.501f,
        0.501f,   0.501f,   0.501f,
        0.501f,   0.501f,   0.501f,
        0.501f,   0.501f,   0.501f
    };

    float rectangle_6_color[] = 
    {
        0.345f,   0.345f,   0.345f,
        0.345f,   0.345f,   0.345f,
        0.345f,   0.345f,   0.345f,
        0.345f,   0.345f,   0.345f,
        0.345f,   0.345f,   0.345f,
        0.345f,   0.345f,   0.345f
    };

    // Code
    vkResult = __createVertexBuffer(0, triangle_position, sizeof(triangle_position), triangle_color, sizeof(triangle_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 0, Reason : %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(1, rectangle_position, sizeof(rectangle_position), rectangle_1_color, sizeof(rectangle_1_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 1 : %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(2, rectangle_position, sizeof(rectangle_position), rectangle_2_color, sizeof(rectangle_2_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 2: %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(3, rectangle_position, sizeof(rectangle_position), rectangle_3_color, sizeof(rectangle_3_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 3: %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(4, rectangle_position, sizeof(rectangle_position), rectangle_4_color, sizeof(rectangle_4_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 4: %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(5, rectangle_5_position, sizeof(rectangle_position), rectangle_5_color, sizeof(rectangle_5_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 5: %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    vkResult = __createVertexBuffer(6, rectangle_position, sizeof(rectangle_position), rectangle_6_color, sizeof(rectangle_6_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Index 6: %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);
    
    return vkResult;
}

VkResult Buildings::createUniformBuffer(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    VkBufferCreateInfo vkBufferCreateInfo;
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = sizeof(VP_UniformData);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    memset((void*)&uniformData, 0, sizeof(UniformData));

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => vkCreateBuffer() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    
    VkMemoryRequirements vkMemoryRequirements;
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, uniformData.vkBuffer, &vkMemoryRequirements);

    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0;

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }

        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => vkAllocateMemory() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
        return vkResult;
    }     

    vkResult = vkBindBufferMemory(vkDevice, uniformData.vkBuffer, uniformData.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => vkBindBufferMemory() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    vkResult = updateUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => updateUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }


    return vkResult;
}

VkResult Buildings::createDescriptorSetLayout(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    //! Initialize VkDescriptorSetLayoutBinding
    VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
    memset((void*)&vkDescriptorSetLayoutBinding, 0, sizeof(VkDescriptorSetLayoutBinding));
    vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vkDescriptorSetLayoutBinding.binding = 0;   //! Mapped with layout(binding = 0) in vertex shader
    vkDescriptorSetLayoutBinding.descriptorCount = 1;
    vkDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vkDescriptorSetLayoutBinding.pImmutableSamplers = NULL;

    //* Step - 3
    VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo;
    memset((void*)&vkDescriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    vkDescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vkDescriptorSetLayoutCreateInfo.pNext = NULL;
    vkDescriptorSetLayoutCreateInfo.flags = 0;
    vkDescriptorSetLayoutCreateInfo.bindingCount = 1;   //! An integer value where you want to bind descriptor set
    vkDescriptorSetLayoutCreateInfo.pBindings = &vkDescriptorSetLayoutBinding;

    //* Step - 4
    vkResult = vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, NULL, &vkDescriptorSetLayout_Buildings);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "BUILDINGS::%s() => vkCreateDescriptorSetLayout() Failed : %d !!!\n", __func__, vkResult);

    return vkResult;
}

VkResult Buildings::createPipelineLayout(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    VkPushConstantRange vkPushConstantRange;
    memset((void*)&vkPushConstantRange, 0, sizeof(VkPushConstantRange));
    vkPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vkPushConstantRange.offset = 0;
    vkPushConstantRange.size = sizeof(glm::mat4);

    //* Step - 3
    VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo;
    memset((void*)&vkPipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));
    vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vkPipelineLayoutCreateInfo.pNext = NULL;
    vkPipelineLayoutCreateInfo.flags = 0;
    vkPipelineLayoutCreateInfo.setLayoutCount = 1;
    vkPipelineLayoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout_Buildings;
    vkPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    vkPipelineLayoutCreateInfo.pPushConstantRanges = &vkPushConstantRange;

    //* Step - 4
    vkResult = vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, NULL, &vkPipelineLayout_Buildings);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "BUILDINGS::%s() => vkCreatePipelineLayout() Failed : %d !!!\n", __func__, vkResult);

    return vkResult;
}

VkResult Buildings::createDescriptorPool(void)
{
    // Variable Declarations
    VkResult vkResult;

    // Code

    //* Vulkan expects decriptor pool size before creating actual descriptor pool
    VkDescriptorPoolSize vkDescriptorPoolSize;
    memset((void*)&vkDescriptorPoolSize, 0, sizeof(VkDescriptorPoolSize));
    vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vkDescriptorPoolSize.descriptorCount = 1;
   
    //* Create the pool
    VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo;
    memset((void*)&vkDescriptorPoolCreateInfo, 0, sizeof(VkDescriptorPoolCreateInfo));
    vkDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    vkDescriptorPoolCreateInfo.pNext = NULL;
    vkDescriptorPoolCreateInfo.flags = 0;
    vkDescriptorPoolCreateInfo.poolSizeCount = 1;
    vkDescriptorPoolCreateInfo.pPoolSizes = &vkDescriptorPoolSize;
    vkDescriptorPoolCreateInfo.maxSets = 1;

    vkResult = vkCreateDescriptorPool(vkDevice, &vkDescriptorPoolCreateInfo, NULL, &vkDescriptorPool_Buildings);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "BUILDINGS::%s() => vkCreateDescriptorPool() Failed : %d !!!\n", __func__, vkResult);  

    return vkResult;
}

VkResult Buildings::createDescriptorSet(void)
{
    // Variable Declarations
    VkResult vkResult;

    // Code

    //* Initialize DescriptorSetAllocationInfo
    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
    memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
    vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    vkDescriptorSetAllocateInfo.pNext = NULL;
    vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool_Buildings;
    vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
    vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout_Buildings;

    vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_Buildings);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => vkAllocateDescriptorSets() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }  
    
    //* Describe whether we want buffer as uniform or image as uniform
    VkDescriptorBufferInfo vkDescriptorBufferInfo;
    memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));
    vkDescriptorBufferInfo.buffer = uniformData.vkBuffer;
    vkDescriptorBufferInfo.offset = 0;
    vkDescriptorBufferInfo.range = sizeof(VP_UniformData);

    /* Update above descriptor set directly to the shader
    There are 2 ways :-
        1) Writing directly to the shader
        2) Copying from one shader to another shader
    */
    VkWriteDescriptorSet vkWriteDescriptorSet;
    memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));
    vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vkWriteDescriptorSet.pNext = NULL;
    vkWriteDescriptorSet.dstSet = vkDescriptorSet_Buildings;
    vkWriteDescriptorSet.dstArrayElement = 0;
    vkWriteDescriptorSet.descriptorCount = 1;
    vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
    vkWriteDescriptorSet.pImageInfo = NULL;
    vkWriteDescriptorSet.pTexelBufferView = NULL;
    vkWriteDescriptorSet.dstBinding = 0;

    vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);

    return vkResult;
}

VkResult Buildings::createPipeline(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    //* Code

    //! Vertex Input State
    VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
    memset((void*)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));
    
    //! Position
    vkVertexInputBindingDescription_array[0].binding = 0;   //! Corresponds to layout(location = 0) in Vertex Shader
    vkVertexInputBindingDescription_array[0].stride = sizeof(float) * 3; 
    vkVertexInputBindingDescription_array[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //! Color
    vkVertexInputBindingDescription_array[1].binding = 1; //! Corresponds to layout(location = 1) in Vertex Shader
    vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 3; 
    vkVertexInputBindingDescription_array[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2];
    memset((void*)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));
    
    //! Position
    vkVertexInputAttributeDescription_array[0].binding = 0;
    vkVertexInputAttributeDescription_array[0].location = 0;
    vkVertexInputAttributeDescription_array[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vkVertexInputAttributeDescription_array[0].offset = 0;
    
    //! Color
    vkVertexInputAttributeDescription_array[1].binding = 1;
    vkVertexInputAttributeDescription_array[1].location = 1;
    vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vkVertexInputAttributeDescription_array[1].offset = 0;

    VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
    memset((void*)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
    vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vkPipelineVertexInputStateCreateInfo.pNext = NULL;
    vkPipelineVertexInputStateCreateInfo.flags = 0;
    vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = _ARRAYSIZE(vkVertexInputBindingDescription_array);
    vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vkVertexInputBindingDescription_array;
    vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
    vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vkVertexInputAttributeDescription_array;
    
    //! Input Assembly State
    VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo;
    memset((void*)&vkPipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vkPipelineInputAssemblyStateCreateInfo.pNext = NULL;
    vkPipelineInputAssemblyStateCreateInfo.flags = 0;
    vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    //! Rasterization State
    VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo;
    memset((void*)&vkPipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
    vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    vkPipelineRasterizationStateCreateInfo.pNext = NULL;
    vkPipelineRasterizationStateCreateInfo.flags = 0;
    vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

    //! Color Blend State
    VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState_array[1];
    memset((void*)vkPipelineColorBlendAttachmentState_array, 0, sizeof(VkPipelineColorBlendAttachmentState) * _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array));
    vkPipelineColorBlendAttachmentState_array[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    vkPipelineColorBlendAttachmentState_array[0].blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo;
    memset((void*)&vkPipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
    vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vkPipelineColorBlendStateCreateInfo.pNext = NULL;
    vkPipelineColorBlendStateCreateInfo.flags = 0;
    vkPipelineColorBlendStateCreateInfo.attachmentCount = _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array);
    vkPipelineColorBlendStateCreateInfo.pAttachments = vkPipelineColorBlendAttachmentState_array;

    //! Viewport Scissor State
    VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo;
    memset((void*)&vkPipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));
    vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vkPipelineViewportStateCreateInfo.pNext = NULL;
    vkPipelineViewportStateCreateInfo.flags = 0;
    vkPipelineViewportStateCreateInfo.viewportCount = 1;    //* We can specify multiple viewports here
    vkPipelineViewportStateCreateInfo.scissorCount = 1;

    //! Viewport Info     
    memset((void*)&vkViewport, 0, sizeof(VkViewport));
    vkViewport.x = 0;
    vkViewport.y = 0;
    vkViewport.width = (float)vkExtent2D_swapchain.width;
    vkViewport.height = (float)vkExtent2D_swapchain.height;
    vkViewport.minDepth = 0.0f;
    vkViewport.maxDepth = 1.0f;

    vkPipelineViewportStateCreateInfo.pViewports = &vkViewport;
    
    //! Scissor Info
    memset((void*)&vkRect2D_Scissor, 0, sizeof(VkRect2D));
    vkRect2D_Scissor.offset.x = 0;
    vkRect2D_Scissor.offset.y = 0;
    vkRect2D_Scissor.extent.width = vkExtent2D_swapchain.width;
    vkRect2D_Scissor.extent.height = vkExtent2D_swapchain.height;
   
    vkPipelineViewportStateCreateInfo.pScissors = &vkRect2D_Scissor;

    //! Depth Stencil State !//
    VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilCreateInfo;
    memset((void*)&vkPipelineDepthStencilCreateInfo, 0, sizeof(VkPipelineDepthStencilStateCreateInfo));
    vkPipelineDepthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    vkPipelineDepthStencilCreateInfo.flags = 0;
    vkPipelineDepthStencilCreateInfo.pNext = NULL;
    vkPipelineDepthStencilCreateInfo.depthTestEnable = VK_TRUE;
    vkPipelineDepthStencilCreateInfo.depthWriteEnable = VK_TRUE;
    vkPipelineDepthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    vkPipelineDepthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
    vkPipelineDepthStencilCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    vkPipelineDepthStencilCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    vkPipelineDepthStencilCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    vkPipelineDepthStencilCreateInfo.stencilTestEnable = VK_FALSE;
    vkPipelineDepthStencilCreateInfo.front = vkPipelineDepthStencilCreateInfo.back;

    //! Dynamic State !//

    //! Multi-Sample State
    VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo;
    memset((void*)&vkPipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
    vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    vkPipelineMultisampleStateCreateInfo.pNext = NULL;
    vkPipelineMultisampleStateCreateInfo.flags = 0;
    vkPipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    //! Shader Stage State
    VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2];
    memset((void*)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));
    
    //* Vertex Shader
    vkPipelineShaderStageCreateInfo_array[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo_array[0].pNext = NULL;
    vkPipelineShaderStageCreateInfo_array[0].flags = 0;
    vkPipelineShaderStageCreateInfo_array[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineShaderStageCreateInfo_array[0].module = vkShaderModule_vertex;
    vkPipelineShaderStageCreateInfo_array[0].pName = "main";
    vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL;

    //* Fragment Shader
    vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
    vkPipelineShaderStageCreateInfo_array[1].flags = 0;
    vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineShaderStageCreateInfo_array[1].module = vkShaderModule_fragment;
    vkPipelineShaderStageCreateInfo_array[1].pName = "main";
    vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL;

    //! Tessellation State !//

    //! As pipelines are created from pipeline caches, we will create VkPipelineCache Object
    VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
    memset((void*)&vkPipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));
    vkPipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vkPipelineCacheCreateInfo.pNext = NULL;
    vkPipelineCacheCreateInfo.flags = 0;
    
    VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;
    vkResult = vkCreatePipelineCache(vkDevice, &vkPipelineCacheCreateInfo, NULL, &vkPipelineCache);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "BUILDINGS::%s() => vkCreatePipelineCache() Failed : %d !!!\n", __func__, vkResult);
    else
        fprintf(gpFile, "BUILDINGS::%s() => vkCreatePipelineCache() Succeeded\n", __func__);

    //! Create actual Graphics Pipeline
    VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo;
    memset((void*)&vkGraphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));
    vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    vkGraphicsPipelineCreateInfo.pNext = NULL;
    vkGraphicsPipelineCreateInfo.flags = 0;
    vkGraphicsPipelineCreateInfo.pVertexInputState = &vkPipelineVertexInputStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pColorBlendState = &vkPipelineColorBlendStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pViewportState = &vkPipelineViewportStateCreateInfo;
    vkGraphicsPipelineCreateInfo.pDepthStencilState = &vkPipelineDepthStencilCreateInfo;
    vkGraphicsPipelineCreateInfo.pDynamicState = NULL;
    vkGraphicsPipelineCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
    vkGraphicsPipelineCreateInfo.stageCount = _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array);
    vkGraphicsPipelineCreateInfo.pStages = vkPipelineShaderStageCreateInfo_array;
    vkGraphicsPipelineCreateInfo.pTessellationState = NULL;
    vkGraphicsPipelineCreateInfo.layout = vkPipelineLayout_Buildings;
    vkGraphicsPipelineCreateInfo.renderPass = vkRenderPass;
    vkGraphicsPipelineCreateInfo.subpass = 0;
    vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    vkGraphicsPipelineCreateInfo.basePipelineIndex = 0;

    vkResult = vkCreateGraphicsPipelines(vkDevice, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, NULL, &vkPipeline_Buildings);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "BUILDINGS::%s() => vkCreateGraphicsPipelines() Failed : %d !!!\n", __func__, vkResult);
    else
        fprintf(gpFile, "BUILDINGS::%s() => vkCreateGraphicsPipelines() Succeeded\n", __func__);

    //* Destroy Pipeline Cache
    if (vkPipelineCache)
    {
        vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
        vkPipelineCache = VK_NULL_HANDLE;
        fprintf(gpFile, "BUILDINGS::%s() => vkDestroyPipelineCache() Succeeded\n", __func__);
    }

    return vkResult;
}

void Buildings::update()
{
    VkResult vkResult = updateUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => updateUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
    }
}

VkResult Buildings::updateUniformBuffer()
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    VP_UniformData mvp_UniformData;
    memset((void*)&mvp_UniformData, 0, sizeof(VP_UniformData));

    glm::mat4 translationMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = glm::mat4(1.0f);

    //* Building 1
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 0.9f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.4f, 0.3f));
    buildings[0].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[0].modelData.modelMatrix = translationMatrix * scaleMatrix;

    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, -0.5f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 1.0f, 0.3f));
    buildings[1].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[1].modelData.modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Building 2
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -0.8f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.7f, 0.3f));
    buildings[2].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[2].modelData.modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Building 3
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 1.0f, 0.3f));
    buildings[3].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[3].modelData.modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Building 4
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.3f, -0.6f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.9f, 0.3f));
    buildings[4].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[4].modelData.modelMatrix = translationMatrix * scaleMatrix;

    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, -0.85f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.65f, 0.3f));
    buildings[5].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[5].modelData.modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Building 5
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(5.8f, -0.8f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 0.7f, 0.3f));
    buildings[6].modelData.modelMatrix = glm::mat4(1.0f);
    buildings[6].modelData.modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    mvp_UniformData.viewMatrix = glm::mat4(1.0f);
    
    glm::mat4 perspectiveProjectionMatrix = glm::mat4(1.0f);
    perspectiveProjectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)winWidth / (float)winHeight,
        0.1f,
        100.0f
    );
    //! 2D Matrix with Column Major (Like OpenGL)
    perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f);
    mvp_UniformData.projectionMatix = perspectiveProjectionMatrix;

    //! Map Uniform Buffer
    void* data = NULL;
    vkResult = vkMapMemory(vkDevice, uniformData.vkDeviceMemory, 0, sizeof(VP_UniformData), 0, &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Uniform Buffer For Buildings : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    //! Copy the data to the mapped buffer (present on device memory)
    memcpy(data, &mvp_UniformData, sizeof(VP_UniformData));

    //! Unmap memory
    vkUnmapMemory(vkDevice, uniformData.vkDeviceMemory);

    return vkResult;
}

void Buildings::buildCommandBuffers(VkCommandBuffer& commandBuffer)
{
    // Code

    //! Bind with Pipeline
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline_Buildings);

    //* Building 1
    //* ----------------------------------------------------------------------------------------
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        vkPipelineLayout_Buildings,
        0,
        1,
        &vkDescriptorSet_Buildings,
        0,
        NULL
    );

    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[0].modelData
    );

    //! Bind with Vertex Position Buffer
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[0].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    VkDeviceSize vkDeviceSize_offset_color[1];
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[0].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[1].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[1].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[1].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 2
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[2].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[2].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[2].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------
    
    //* Building 3
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[3].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[3].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[3].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 4
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[4].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[4].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[4].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 5
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[5].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[5].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[5].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 6
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        commandBuffer,
        vkPipelineLayout_Buildings,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[6].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        0, 
        1, 
        &buildings[6].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        commandBuffer, 
        1, 
        1, 
        &buildings[6].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------
}

VkResult Buildings::resize(int width, int height)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    //? DESTROY
    //?--------------------------------------------------------------------------------------------------
    //* Wait for device to complete in-hand tasks
    if (vkDevice)
        vkDeviceWaitIdle(vkDevice);

    //* Destroy PipelineLayout
    if (vkPipelineLayout_Buildings)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Buildings, NULL);
        vkPipelineLayout_Buildings = VK_NULL_HANDLE;
    }

    //* Destroy Pipeline
    if (vkPipeline_Buildings)
    {
        vkDestroyPipeline(vkDevice, vkPipeline_Buildings, NULL);
        vkPipeline_Buildings = VK_NULL_HANDLE;
    }
    //?--------------------------------------------------------------------------------------------------
    
    //? RECREATE FOR RESIZE
    //?--------------------------------------------------------------------------------------------------
    //* Create Pipeline Layout
    vkResult = createPipelineLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => createPipelineLayout() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    //* Create Pipeline
    vkResult = createPipeline();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "BUILDINGS::%s() => createPipeline() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    //?--------------------------------------------------------------------------------------------------

    return vkResult;
}

Buildings::~Buildings()
{
    if (vkDevice)
        vkDeviceWaitIdle(vkDevice);

    if (vkPipelineLayout_Buildings)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Buildings, NULL);
        vkPipelineLayout_Buildings = VK_NULL_HANDLE;
    }

    if (vkPipeline_Buildings)
    {
        vkDestroyPipeline(vkDevice, vkPipeline_Buildings, NULL);
        vkPipeline_Buildings = VK_NULL_HANDLE;
        fprintf(gpFile, "BUILDINGS::%s() => vkDestroyPipeline() Succeeded\n", __func__);
    }

    if (vkDescriptorPool_Buildings)
    {
        vkDestroyDescriptorPool(vkDevice, vkDescriptorPool_Buildings, NULL);
        vkDescriptorPool_Buildings = VK_NULL_HANDLE;
        vkDescriptorSet_Buildings = VK_NULL_HANDLE;
        fprintf(gpFile, "BUILDINGS::%s() => vkDestroyDescriptorPool() => Destroyed vkDescriptorPool_Buildings and vkDescriptorSet_Buildings Successfully\n", __func__);
    }

    if (vkPipelineLayout_Buildings)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Buildings, NULL);
        vkPipelineLayout_Buildings = VK_NULL_HANDLE;
        fprintf(gpFile, "BUILDINGS::%s() => vkDestroyPipelineLayout() Succeeded\n", __func__);
    }

    if (vkDescriptorSetLayout_Buildings)
	{
		vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout_Buildings, NULL);
		vkDescriptorSetLayout_Buildings = VK_NULL_HANDLE;
        fprintf(gpFile, "BUILDINGS::%s() => vkDestroyDescriptorSetLayout() Succeeded\n", __func__);
	}

    for (int i = BUILDING_GEOMETRY_COUNT - 1; i >= 0; i--)
    {
        if (buildings[i].vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, buildings[i].vertexData_color.vkDeviceMemory, NULL);
            buildings[i].vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
        }

        if (buildings[i].vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, buildings[i].vertexData_color.vkBuffer, NULL);
            buildings[i].vertexData_color.vkBuffer = VK_NULL_HANDLE;
        }

        if (buildings[i].vertexData_position.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, buildings[i].vertexData_position.vkDeviceMemory, NULL);
            buildings[i].vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
        }

        if (buildings[i].vertexData_position.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, buildings[i].vertexData_position.vkBuffer, NULL);
            buildings[i].vertexData_position.vkBuffer = VK_NULL_HANDLE;
        }
    }

    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_vertex);
    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_fragment);
}
