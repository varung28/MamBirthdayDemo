#include "Buildings.h"

// Global Variable Declarations
typedef struct 
{
    VertexData vertexData_position;
    VertexData vertexData_color;
    UniformData uniformData;
    VkDescriptorSet vkDescriptorSet;
    float color;
} Buildings;

Buildings buildings[5];


//! INTERNAL FUNCTIONS
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VkResult __createVertexBuffer(int index, float* position, float* color)
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
    vkBufferCreateInfo.size = 18 * sizeof(float);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &buildings[index].vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Vertex Position Buffer For Building Index : %d\n", __func__, index);
    
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
    else
        fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Vertex Position Buffer For Building Index : %d\n", __func__, index);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, buildings[index].vertexData_position.vkBuffer, buildings[index].vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Vertex Position Buffer For Building Index : %d\n", __func__, index);

    //* Step - 11
    vkResult = vkMapMemory(vkDevice, buildings[index].vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Position Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkMapMemory() Succeeded For Vertex Position Buffer For Building Index : %d\n", __func__, index);

    //* Step - 12
    memcpy(data, position, 18 * sizeof(float));

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
    vkBufferCreateInfo.size = 18 * sizeof(float);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &buildings[index].vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Vertex Color Buffer For Building Index : %d\n", __func__, index);
    
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
    else
        fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Vertex Color Buffer For Building Index : %d\n", __func__, index);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, buildings[index].vertexData_color.vkBuffer, buildings[index].vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Vertex Color Buffer For Building Index : %d\n", __func__, index);

    //* Step - 11
    data = NULL;
    vkResult = vkMapMemory(vkDevice, buildings[index].vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Color Buffer For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkMapMemory() Succeeded For Vertex Color Buffer For Building Index : %d\n", __func__, index);

    //* Step - 12
    memcpy(data, color, 18 * sizeof(float));

    //* Step - 13
    vkUnmapMemory(vkDevice, buildings[index].vertexData_color.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

VkResult __createUniformBuffer(int index)
{
    // Variable Declarations
    VkResult vkResult;

    VkBufferCreateInfo vkBufferCreateInfo;
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = sizeof(MVP_UniformData);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    memset((void*)&buildings[index].uniformData, 0, sizeof(UniformData));

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &buildings[index].uniformData.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Uniform Data Used For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
        return vkResult;
    }
    else
        fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Uniform Data Used For Building Index : %d\n", __func__, index);
    
    VkMemoryRequirements vkMemoryRequirements;
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, buildings[index].uniformData.vkBuffer, &vkMemoryRequirements);

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

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &buildings[index].uniformData.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Uniform Data Used For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
        return vkResult;
    }     
    else
        fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Uniform Data Used For Building Index : %d\n", __func__, index);

    vkResult = vkBindBufferMemory(vkDevice, buildings[index].uniformData.vkBuffer, buildings[index].uniformData.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Uniform Data Used For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
        return vkResult;
    }
    else
        fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Uniform Data Used For Building Index : %d\n", __func__, index);

    return vkResult;
}

VkResult __updateUniformBuffer(int index, MVP_UniformData* mvp_UniformData)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;
    void* data = NULL;

    // Code

    //! Map Uniform Buffer
    vkResult = vkMapMemory(vkDevice, buildings[index].uniformData.vkDeviceMemory, 0, sizeof(MVP_UniformData), 0, &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Uniform Buffer For Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
        return vkResult;
    }

    //! Copy the data to the mapped buffer (present on device memory)
    memcpy(data, &mvp_UniformData, sizeof(MVP_UniformData));

    //! Unmap memory
    vkUnmapMemory(vkDevice, buildings[index].uniformData.vkDeviceMemory);

    return vkResult;
}

VkResult __createDescriptorSet(int index)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code

    //* Initialize DescriptorSetAllocationInfo
    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
    memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
    vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    vkDescriptorSetAllocateInfo.pNext = NULL;
    vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
    vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
    vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

    vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &buildings[index].vkDescriptorSet);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => vkAllocateDescriptorSets() Failed For Building Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
        return vkResult;
    }  
    else
        fprintf(gpFile, "%s() => vkAllocateDescriptorSets() Succeeded For Building Index : %d\n", __func__, index);
    
    //* Describe whether we want buffer as uniform or image as uniform
    VkDescriptorBufferInfo vkDescriptorBufferInfo;
    memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));
    vkDescriptorBufferInfo.buffer = buildings[index].uniformData.vkBuffer;
    vkDescriptorBufferInfo.offset = 0;
    vkDescriptorBufferInfo.range = sizeof(MVP_UniformData);

    /* Update above descriptor set directly to the shader
    There are 2 ways :-
        1) Writing directly to the shader
        2) Copying from one shader to another shader
    */
    VkWriteDescriptorSet vkWriteDescriptorSet;
    memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));
    vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vkWriteDescriptorSet.pNext = NULL;
    vkWriteDescriptorSet.dstSet = buildings[index].vkDescriptorSet;
    vkWriteDescriptorSet.dstArrayElement = 0;
    vkWriteDescriptorSet.descriptorCount = 1;
    vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
    vkWriteDescriptorSet.pImageInfo = NULL;
    vkWriteDescriptorSet.pTexelBufferView = NULL;
    vkWriteDescriptorSet.dstBinding = 0;

    vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);
    //! -------------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

void __buildCommandBuffers(int swapchainImageIndex, int index)
{
    //! Bind the Descriptor Set to the Pipeline
    vkCmdBindDescriptorSets(
        vkCommandBuffer_array[swapchainImageIndex],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        vkPipelineLayout,
        0,
        1,
        &buildings[index].vkDescriptorSet,
        0,
        NULL
    );

    //! Bind with Vertex Position Buffer
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[index].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    VkDeviceSize vkDeviceSize_offset_color[1];
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[index].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//! EXPORT FUNCTIONS
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VkResult createVertexBuffer_buildings(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    float rectangle_position[] = 
    {
        // Triangle 1
        1.0f,   1.0f,   0.0f,   // Top Right
        -1.0f,  1.0f,   0.0f,   // Top Left
        -1.0f,  -1.0f,  0.0f,   // Bottom Left

        // Triangle 2
        -1.0f,  -1.0f,  0.0f,   // Bottom Left
        1.0f,   -1.0f,  0.0f,   // Bottom Right
        1.0f,   1.0f,   0.0f,   // Top Right
    };

    float rectangle_color[] = 
    {
        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   1.0f,

        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   1.0f
    };

    // Code
    return __createVertexBuffer(0, rectangle_position, rectangle_color);
}

VkResult createUniformBuffer_buildings(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    vkResult = __createUniformBuffer(0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createUniformBuffer() Failed For For Building Index : %d, Reason : %d !!!\n", __func__, 0, vkResult);
        return vkResult;
    }
    else
        fprintf(gpFile, "%s() => __createUniformBuffer() Succeeded For For Building Index : %d\n", __func__, 0);

    vkResult = updateUniformBuffer_buildings();
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => updateUniformBuffer_buildings() Failed : %d !!!\n", __func__, vkResult);
        return vkResult;
    }
    else
        fprintf(gpFile, "%s() => updateUniformBuffer_buildings() Succeeded\n", __func__);

    return vkResult;
}

VkResult updateUniformBuffer_buildings(void)
{
    // Code
    MVP_UniformData mvp_UniformData;
    memset((void*)&mvp_UniformData, 0, sizeof(MVP_UniformData));

    //! Update Matrices
    mvp_UniformData.modelMatrix = glm::mat4(1.0f);
    mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
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

    return __updateUniformBuffer(0, &mvp_UniformData);
}

VkResult createDescriptorSet_buildings(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    vkResult = __createDescriptorSet(0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createDescriptorSet() Failed For Building Index : %d, Reason : %d !!!\n", __func__, 0, vkResult);
        return vkResult;
    }  
    else
        fprintf(gpFile, "%s() => __createDescriptorSet() Succeeded For Building Index : %d\n", __func__, 0);
    
    return vkResult;
}

void buildCommandBuffers_buildings(int swapchainImageIndex)
{
    // Code
    __buildCommandBuffers(swapchainImageIndex, 0);
}

void uninitialize_buildings(void)
{
    // Code
    for (int i = BUILDING_COUNT - 1; i <= 0; i++)
    {
        buildings[i].vkDescriptorSet = VK_NULL_HANDLE;
        
        if (buildings[i].uniformData.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, buildings[i].uniformData.vkDeviceMemory, NULL);
            buildings[i].uniformData.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkFreeMemory() Succeeded For Uniform Buffer, Building Index : %d\n", __func__, i);
        }

        if (buildings[i].uniformData.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, buildings[i].uniformData.vkBuffer, NULL);
            buildings[i].uniformData.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkDestroyBuffer() Succeeded For Uniform Buffer, Building Index : %d\n", __func__, i);
        }

        if (buildings[i].vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, buildings[i].vertexData_color.vkDeviceMemory, NULL);
            buildings[i].vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkFreeMemory() Succeeded For Color Buffer, Building Index : %d\n", __func__, i);
        }

        if (buildings[i].vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, buildings[i].vertexData_color.vkBuffer, NULL);
            buildings[i].vertexData_color.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkDestroyBuffer() Succeeded For Color Buffer, Building Index : %d\n", __func__, i);
        }

        if (buildings[i].vertexData_position.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, buildings[i].vertexData_position.vkDeviceMemory, NULL);
            buildings[i].vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkFreeMemory() Succeeded For Position Buffer, Building Index : %d\n", __func__, i);
        }

        if (buildings[i].vertexData_position.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, buildings[i].vertexData_position.vkBuffer, NULL);
            buildings[i].vertexData_position.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkDestroyBuffer() Succeeded For Position Buffer, Building Index : %d\n", __func__, i);
        }
    }
    
}
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------