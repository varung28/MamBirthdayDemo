#include "Buildings.h"

#define BUILDING_GEOMETRY_COUNT  7

// Global Variable Declarations
//? Vertex Buffer Related Variables
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
    PushData modelData;
} Buildings;

Buildings buildings[7];

extern UniformData uniformData;

//! INTERNAL FUNCTIONS
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VkResult __createVertexBuffer(int index, float* position, int positionSize, float* color, int colorSize)
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
    memcpy(data, color, colorSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, buildings[index].vertexData_color.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

void __buildCommandBuffers(int swapchainImageIndex)
{
    //* Building 1
    //* ----------------------------------------------------------------------------------------
    vkCmdBindDescriptorSets(
        vkCommandBuffer_array[swapchainImageIndex],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        vkPipelineLayout,
        0,
        1,
        &vkDescriptorSet,
        0,
        NULL
    );

    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[0].modelData
    );

    //! Bind with Vertex Position Buffer
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[0].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    VkDeviceSize vkDeviceSize_offset_color[1];
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[0].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 3, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[1].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[1].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[1].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 2
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[2].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[2].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[2].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------
    
    //* Building 3
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[3].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[3].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[3].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 4
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[4].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[4].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[4].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 5
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[5].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[5].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[5].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------

    //* Building 6
    //* ----------------------------------------------------------------------------------------
    vkCmdPushConstants(
        vkCommandBuffer_array[swapchainImageIndex],
        vkPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(PushData),
        &buildings[6].modelData
    );

    //! Bind with Vertex Position Buffer
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &buildings[6].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &buildings[6].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], 6, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------
}
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//! EXPORT FUNCTIONS
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VkResult createVertexBuffer_buildings(void)
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

VkResult updateUniformBuffer_buildings(void)
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

void buildCommandBuffers_buildings(int swapchainImageIndex)
{
    // Code
    __buildCommandBuffers(swapchainImageIndex);
}

void uninitialize_buildings(void)
{
    // Code
     for (int i = BUILDING_GEOMETRY_COUNT - 1; i >= 0; i--)
    {
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