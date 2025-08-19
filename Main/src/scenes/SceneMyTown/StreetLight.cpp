#include "StreetLight.h"

extern int winWidth;
extern int winHeight;

bool updateColorAnimation = false;

StreetLight::StreetLight()
{
    vkShaderModule_vertex = ShaderModuleHelper::LoadShaderModule("bin\\PushConstant.vert.spv");
    vkShaderModule_fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader.frag.spv"); 
}

VkResult StreetLight::__createVertexBuffer(int index, float *position, int positionSize, float *color, int colorSize)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;
    VkBufferCreateInfo vkBufferCreateInfo;
    VkMemoryRequirements vkMemoryRequirements;
    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    void *data = NULL;

    //! Vertex Position
    //! ---------------------------------------------------------------------------------------------------------------------------------
    //* Step - 4
    memset((void *)&lightData[index].vertexData_position, 0, sizeof(VertexData));

    //* Step - 5

    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0; //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = positionSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &lightData[index].vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Position Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 7
    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, lightData[index].vertexData_position.vkBuffer, &vkMemoryRequirements);

    //* Step - 8
    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
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
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &lightData[index].vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Position Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, lightData[index].vertexData_position.vkBuffer, lightData[index].vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Position Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 11
    vkResult = vkMapMemory(vkDevice, lightData[index].vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Position Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 12
    memcpy(data, position, positionSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, lightData[index].vertexData_position.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    //! Vertex Color
    //! ---------------------------------------------------------------------------------------------------------------------------------
    //* Step - 4
    memset((void *)&lightData[index].vertexData_color, 0, sizeof(VertexData));

    //* Step - 5
    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0; //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = colorSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &lightData[index].vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Color Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 7
    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, lightData[index].vertexData_color.vkBuffer, &vkMemoryRequirements);

    //* Step - 8
    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
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
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &lightData[index].vertexData_color.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Color Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, lightData[index].vertexData_color.vkBuffer, lightData[index].vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Color Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 11
    data = NULL;
    vkResult = vkMapMemory(vkDevice, lightData[index].vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Color Buffer For Light Object Index Index : %d, Reason : %d !!!\n", __func__, index, vkResult);

    //* Step - 12
    memcpy(data, color, colorSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, lightData[index].vertexData_color.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

VkResult StreetLight::initialize()
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    vkResult = createVertexBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createVertexBuffer() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createDescriptorSetLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createDescriptorSetLayout() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createPipelineLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createPipelineLayout() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createDescriptorPool();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createDescriptorPool() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createDescriptorSet();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createDescriptorSet() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    vkResult = createPipeline();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::ERROR : %s() => createPipeline() Failed : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }

    return vkResult;
}

VkResult StreetLight::createVertexBuffer(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    float street_lamp_middle_bar_position[] = 
    {
        1.0f,  1.0f, 0.0f, // right top 
        -1.0f,  1.0f, 0.0f, // left top
        -1.0f, -1.0f, 0.0f, // left bottom

        // second triangle
        -1.0f, -1.0f, 0.0f, // left bottom
        1.0f, -1.0f, 0.0f, // right bottom
        1.0f,  1.0f, 0.0f // right top
	};

	float street_lamp_middle_bar_color[] = 
    {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
	};

	float street_lamp_light_position[] = 
    {
        // first triangle
        1.0f,  1.0f, 0.0f, // right top 
        -1.0f,  1.0f, 0.0f, // left top
        -1.0f, -1.0f, 0.0f, // left bottom

        // second triangle
        -1.0f, -1.0f, 0.0f, // left bottom
        1.0f, -1.0f, 0.0f, // right bottom
        1.0f,  1.0f, 0.0f // right top
	};

	float street_lamp_light_color[] = 
    {
        0.5098f, 0.4863f, 0.5059f,					  
        0.5098f, 0.4863f, 0.5059f,					  
        0.5098f, 0.4863f, 0.5059f,					  
        0.5098f, 0.4863f, 0.5059f,					  
        0.5098f, 0.4863f, 0.5059f,					  
        0.5098f, 0.4863f, 0.5059f,
	};

	float street_lamp_light_color_yellow[] = 
    {
        0.949f, 0.980f, 0.0745f,					  
        0.949f, 0.980f, 0.0745f,					  
        0.949f, 0.980f, 0.0745f,					  
        0.949f, 0.980f, 0.0745f,					  
        0.949f, 0.980f, 0.0745f,					  
        0.949f, 0.980f, 0.0745f,					  
	};

    vkResult = __createVertexBuffer(0, street_lamp_middle_bar_position, sizeof(street_lamp_middle_bar_position), street_lamp_middle_bar_color, sizeof(street_lamp_middle_bar_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Middle Bar : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    vkResult = __createVertexBuffer(1, street_lamp_light_position, sizeof(street_lamp_light_position), street_lamp_light_color, sizeof(street_lamp_light_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Lamp Light : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    
    vkResult = __createVertexBuffer(2, street_lamp_light_position, sizeof(street_lamp_light_position), street_lamp_light_color_yellow, sizeof(street_lamp_light_color_yellow));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Lamp Light Yellow Color : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    
    
    return vkResult;
}

VkResult StreetLight::createUniformBuffer(void)
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
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreateBuffer() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
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
        fprintf(gpFile, "STREET_LIGHT::%s() => vkAllocateMemory() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
        return vkResult;
    }     

    vkResult = vkBindBufferMemory(vkDevice, uniformData.vkBuffer, uniformData.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => vkBindBufferMemory() Failed For Uniform Data : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    vkResult = updateUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => updateUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    return vkResult;
}

VkResult StreetLight::createDescriptorSetLayout(void)
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
    vkResult = vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, NULL, &vkDescriptorSetLayout_Lights);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreateDescriptorSetLayout() Failed : %d !!!\n", __func__, vkResult);

    return vkResult;
}

VkResult StreetLight::createPipelineLayout(void)
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
    vkPipelineLayoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout_Lights;
    vkPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    vkPipelineLayoutCreateInfo.pPushConstantRanges = &vkPushConstantRange;

    //* Step - 4
    vkResult = vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, NULL, &vkPipelineLayout_Lights);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreatePipelineLayout() Failed : %d !!!\n", __func__, vkResult);

    return vkResult;
}

VkResult StreetLight::createDescriptorPool(void)
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

    vkResult = vkCreateDescriptorPool(vkDevice, &vkDescriptorPoolCreateInfo, NULL, &vkDescriptorPool_Lights);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreateDescriptorPool() Failed : %d !!!\n", __func__, vkResult);  

    return vkResult;
}

VkResult StreetLight::createDescriptorSet(void)
{
    // Variable Declarations
    VkResult vkResult;

    // Code

    //* Initialize DescriptorSetAllocationInfo
    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
    memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
    vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    vkDescriptorSetAllocateInfo.pNext = NULL;
    vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool_Lights;
    vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
    vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout_Lights;

    vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_Lights);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => vkAllocateDescriptorSets() Failed : %d !!!\n", __func__, vkResult);
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
    vkWriteDescriptorSet.dstSet = vkDescriptorSet_Lights;
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

VkResult StreetLight::createPipeline(void)
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
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreatePipelineCache() Failed : %d !!!\n", __func__, vkResult);

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
    vkGraphicsPipelineCreateInfo.layout = vkPipelineLayout_Lights;
    vkGraphicsPipelineCreateInfo.renderPass = vkRenderPass;
    vkGraphicsPipelineCreateInfo.subpass = 0;
    vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    vkGraphicsPipelineCreateInfo.basePipelineIndex = 0;

    vkResult = vkCreateGraphicsPipelines(vkDevice, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, NULL, &vkPipeline_Lights);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "STREET_LIGHT::%s() => vkCreateGraphicsPipelines() Failed : %d !!!\n", __func__, vkResult);

    //* Destroy Pipeline Cache
    if (vkPipelineCache)
    {
        vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
        vkPipelineCache = VK_NULL_HANDLE;
    }

    return vkResult;
}

void StreetLight::update()
{
    VkResult vkResult = updateUniformBuffer();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => updateUniformBuffer() Failed : %d !!!\n", __func__, vkResult);
    }
}

VkResult StreetLight::updateUniformBuffer()
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    VP_UniformData mvp_UniformData;
    memset((void*)&mvp_UniformData, 0, sizeof(VP_UniformData));

    glm::mat4 translationMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = glm::mat4(1.0f);

    //* Light 1
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-5.8f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[0][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[0][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-5.7f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[0][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[0][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-5.4f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[0][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[0][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-5.8f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[0][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[0][3].modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Light 2
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.2f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[1][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[1][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.93f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[1][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[1][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.23f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[1][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[1][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.2f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[1][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[1][3].modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Light 3
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[2][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[2][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[2][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[2][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.33f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[2][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[2][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[2][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[2][3].modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Light 4
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[3][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[3][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[3][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[3][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[3][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[3][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[3][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[3][3].modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Light 5
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.6f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[4][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[4][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.59f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[4][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[4][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.29f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[4][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[4][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.6f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[4][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[4][3].modelMatrix = translationMatrix * scaleMatrix;
    //* ----------------------------------------------------------------------------------------

    //* Light 6
    //* ----------------------------------------------------------------------------------------
    //! Middle Bar (Vertical)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 0.25f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.25f, 0.4f));
    lightsModelData[5][0].modelMatrix = glm::mat4(1.0f);
    lightsModelData[5][0].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 1
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.49f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[5][1].modelMatrix = glm::mat4(1.0f);
    lightsModelData[5][1].modelMatrix = translationMatrix * scaleMatrix;

    //! Lamp 2
    translationMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.16f, 0.5f, -11.5f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.06f, 0.0f));
    lightsModelData[5][2].modelMatrix = glm::mat4(1.0f);
    lightsModelData[5][2].modelMatrix = translationMatrix * scaleMatrix;

    //! Top Bar (Horizontal)
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 0.52f, -12.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.02f, 0.0f));
    lightsModelData[5][3].modelMatrix = glm::mat4(1.0f);
    lightsModelData[5][3].modelMatrix = translationMatrix * scaleMatrix;
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
        fprintf(gpFile, "STREET_LIGHT::%s() => vkMapMemory() Failed For Uniform Buffer For Buildings : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    //! Copy the data to the mapped buffer (present on device memory)
    memcpy(data, &mvp_UniformData, sizeof(VP_UniformData));

    //! Unmap memory
    vkUnmapMemory(vkDevice, uniformData.vkDeviceMemory);

    return vkResult;
}

void StreetLight::buildCommandBuffers(VkCommandBuffer& commandBuffer)
{
    // Code
    
    //! Bind with Pipeline
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline_Lights);

    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        vkPipelineLayout_Lights,
        0,
        1,
        &vkDescriptorSet_Lights,
        0,
        NULL
    );

    for (int i = 0; i < LIGHT_GEOMETRY_COUNT; i++)
    {
        //! Middle Bar
        vkCmdPushConstants(
            commandBuffer,
            vkPipelineLayout_Lights,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(PushData),
            &lightsModelData[i][0]
        );

        //! Bind with Vertex Position Buffer
        VkDeviceSize vkDeviceSize_offset_position[1];
        memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            0, 
            1, 
            &lightData[0].vertexData_position.vkBuffer, 
            vkDeviceSize_offset_position
        );

        //! Bind with Vertex Color Buffer
        VkDeviceSize vkDeviceSize_offset_color[1];
        memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            1, 
            1, 
            &lightData[0].vertexData_color.vkBuffer, 
            vkDeviceSize_offset_color
        );

        //! Vulkan Drawing Function
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);

        //! Lamp
        vkCmdPushConstants(
            commandBuffer,
            vkPipelineLayout_Lights,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(PushData),
            &lightsModelData[i][1]
        );

        //! Bind with Vertex Position Buffer
        memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            0, 
            1, 
            &lightData[1].vertexData_position.vkBuffer, 
            vkDeviceSize_offset_position
        );

        memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
        if (updateColorAnimation)
        {
            //! Yellow Lights
            vkCmdBindVertexBuffers(
                commandBuffer, 
                1, 
                1, 
                &lightData[2].vertexData_color.vkBuffer, 
                vkDeviceSize_offset_color
            );
        }
        else
        {
            vkCmdBindVertexBuffers(
                commandBuffer, 
                1, 
                1, 
                &lightData[1].vertexData_color.vkBuffer, 
                vkDeviceSize_offset_color
            );
        }

        //! Vulkan Drawing Function
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);

        //! Lamp
        vkCmdPushConstants(
            commandBuffer,
            vkPipelineLayout_Lights,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(PushData),
            &lightsModelData[i][2]
        );

        //! Bind with Vertex Position Buffer
        memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            0, 
            1, 
            &lightData[1].vertexData_position.vkBuffer, 
            vkDeviceSize_offset_position
        );

        memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
        if (updateColorAnimation)
        {
            //! Yellow Lights
            vkCmdBindVertexBuffers(
                commandBuffer, 
                1, 
                1, 
                &lightData[2].vertexData_color.vkBuffer, 
                vkDeviceSize_offset_color
            );
        }
        else
        {
            vkCmdBindVertexBuffers(
                commandBuffer, 
                1, 
                1, 
                &lightData[1].vertexData_color.vkBuffer, 
                vkDeviceSize_offset_color
            );
        }

        //! Vulkan Drawing Function
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);

        //! Top Bar
        vkCmdPushConstants(
            commandBuffer,
            vkPipelineLayout_Lights,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(PushData),
            &lightsModelData[i][3]
        );

        //! Bind with Vertex Position Buffer
        memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            0, 
            1, 
            &lightData[0].vertexData_position.vkBuffer, 
            vkDeviceSize_offset_position
        );

        //! Bind with Vertex Color Buffer
        memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
        vkCmdBindVertexBuffers(
            commandBuffer, 
            1, 
            1, 
            &lightData[0].vertexData_color.vkBuffer, 
            vkDeviceSize_offset_color
        );

        //! Vulkan Drawing Function
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }
}

VkResult StreetLight::resize(int width, int height)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    //? DESTROY
    //?--------------------------------------------------------------------------------------------------
    //* Wait for device to complete in-hand tasks
    if (vkDevice)
        vkDeviceWaitIdle(vkDevice);

    //* Destroy PipelineLayout
    if (vkPipelineLayout_Lights)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Lights, NULL);
        vkPipelineLayout_Lights = VK_NULL_HANDLE;
    }

    //* Destroy Pipeline
    if (vkPipeline_Lights)
    {
        vkDestroyPipeline(vkDevice, vkPipeline_Lights, NULL);
        vkPipeline_Lights = VK_NULL_HANDLE;
    }
    //?--------------------------------------------------------------------------------------------------
    
    //? RECREATE FOR RESIZE
    //?--------------------------------------------------------------------------------------------------
    //* Create Pipeline Layout
    vkResult = createPipelineLayout();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => createPipelineLayout() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }

    //* Create Pipeline
    vkResult = createPipeline();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "STREET_LIGHT::%s() => createPipeline() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    //?--------------------------------------------------------------------------------------------------

    return vkResult;
}

StreetLight::~StreetLight()
{
    if (vkDevice)
        vkDeviceWaitIdle(vkDevice);

    if (vkPipelineLayout_Lights)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Lights, NULL);
        vkPipelineLayout_Lights = VK_NULL_HANDLE;
    }

    if (vkPipeline_Lights)
    {
        vkDestroyPipeline(vkDevice, vkPipeline_Lights, NULL);
        vkPipeline_Lights = VK_NULL_HANDLE;
    }

    if (vkDescriptorPool_Lights)
    {
        vkDestroyDescriptorPool(vkDevice, vkDescriptorPool_Lights, NULL);
        vkDescriptorPool_Lights = VK_NULL_HANDLE;
        vkDescriptorSet_Lights = VK_NULL_HANDLE;
    }

    if (vkPipelineLayout_Lights)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout_Lights, NULL);
        vkPipelineLayout_Lights = VK_NULL_HANDLE;
    }

    if (vkDescriptorSetLayout_Lights)
	{
		vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout_Lights, NULL);
		vkDescriptorSetLayout_Lights = VK_NULL_HANDLE;
	}

    if (uniformData.vkDeviceMemory)
    {
        vkFreeMemory(vkDevice, uniformData.vkDeviceMemory, NULL);
        uniformData.vkDeviceMemory = VK_NULL_HANDLE;
    }

    if (uniformData.vkBuffer)
    {
        vkDestroyBuffer(vkDevice, uniformData.vkBuffer, NULL);
        uniformData.vkBuffer = VK_NULL_HANDLE;
    }

    for (int i = 1; i >= 0; i--)
    {
        if (lightData[i].vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, lightData[i].vertexData_color.vkDeviceMemory, NULL);
            lightData[i].vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
        }

        if (lightData[i].vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, lightData[i].vertexData_color.vkBuffer, NULL);
            lightData[i].vertexData_color.vkBuffer = VK_NULL_HANDLE;
        }

        if (lightData[i].vertexData_position.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, lightData[i].vertexData_position.vkDeviceMemory, NULL);
            lightData[i].vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
        }

        if (lightData[i].vertexData_position.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, lightData[i].vertexData_position.vkBuffer, NULL);
            lightData[i].vertexData_position.vkBuffer = VK_NULL_HANDLE;
        }
    }

    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_vertex);
    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_fragment);
}
