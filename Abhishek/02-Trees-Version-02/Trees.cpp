#include "Trees.h"

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
} Trees;

Trees trees[7];

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
    memset((void*)&trees[index].vertexData_position, 0, sizeof(VertexData));

    //* Step - 5
    
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;   //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = positionSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &trees[index].vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Position Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Vertex Position Buffer For Tree Index : %d\n", __func__, index);
    
    //* Step - 7
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, trees[index].vertexData_position.vkBuffer, &vkMemoryRequirements);

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
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &trees[index].vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Position Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Vertex Position Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, trees[index].vertexData_position.vkBuffer, trees[index].vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Position Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Vertex Position Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 11
    vkResult = vkMapMemory(vkDevice, trees[index].vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Position Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkMapMemory() Succeeded For Vertex Position Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 12
    memcpy(data, position, positionSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, trees[index].vertexData_position.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    //! Vertex Color
    //! ---------------------------------------------------------------------------------------------------------------------------------
    //* Step - 4
    memset((void*)&trees[index].vertexData_color, 0, sizeof(VertexData));

    //* Step - 5
    memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.flags = 0;   //! Valid Flags are used in sparse(scattered) buffers
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.size = colorSize;
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    //* Step - 6
    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &trees[index].vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Color Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Vertex Color Buffer For Tree Index : %d\n", __func__, index);
    
    //* Step - 7
    memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, trees[index].vertexData_color.vkBuffer, &vkMemoryRequirements);

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
    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &trees[index].vertexData_color.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Color Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Vertex Color Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 10
    //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
    vkResult = vkBindBufferMemory(vkDevice, trees[index].vertexData_color.vkBuffer, trees[index].vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Color Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Vertex Color Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 11
    data = NULL;
    vkResult = vkMapMemory(vkDevice, trees[index].vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
        fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Color Buffer For Tree Index : %d, Reason : %d !!!\n", __func__, index, vkResult);
    else
        fprintf(gpFile, "%s() => vkMapMemory() Succeeded For Vertex Color Buffer For Tree Index : %d\n", __func__, index);

    //* Step - 12
    memcpy(data, color, colorSize);

    //* Step - 13
    vkUnmapMemory(vkDevice, trees[index].vertexData_color.vkDeviceMemory);
    //! ---------------------------------------------------------------------------------------------------------------------------------

    return vkResult;
}

void __buildCommandBuffers(int swapchainImageIndex)
{
    //* Tree 1
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
        &trees[0].modelData
    );

    //! Bind with Vertex Position Buffer
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        0, 
        1, 
        &trees[0].vertexData_position.vkBuffer, 
        vkDeviceSize_offset_position
    );

    //! Bind with Vertex Color Buffer
    VkDeviceSize vkDeviceSize_offset_color[1];
    memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(
        vkCommandBuffer_array[swapchainImageIndex], 
        1, 
        1, 
        &trees[0].vertexData_color.vkBuffer, 
        vkDeviceSize_offset_color
    );

    //! Vulkan Drawing Function
    vkCmdDraw(vkCommandBuffer_array[swapchainImageIndex], TREES_DRAW_VERTEX_COUNT, 1, 0, 0);
    //* ----------------------------------------------------------------------------------------
}
//! ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


VkResult createVertexBuffer_trees(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    float triangle_position[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,

        0.8f, -1.0f, 0.0f,
        -0.8f, -1.0f, 0.0f, 
        -1.2f, -2.0f, 0.0f,

        0.8f, -1.0f, 0.0f,
        -1.2f, -2.0f, 0.0f,
        1.2f, -2.0f, 0.0f,

        1.0f, -2.0f, 0.0f,
        -1.0f, -2.0f, 0.0f,
        -1.4f, -3.0f, 0.0f,

        1.0f, -2.0f, 0.0f,
        -1.4f, -3.0f, 0.0f,
        1.4f, -3.0f, 0.0f,

        1.2f, -3.0f, 0.0f,
        -1.2f, -3.0f, 0.0f,
        -1.6f, -4.0f, 0.0f,

        1.2f, -3.0f, 0.0f,
        -1.6f, -4.0f, 0.0f,
        1.6f, -4.0f, 0.0f,

        0.3f, -4.0f, 0.0f,
        -0.3f, -4.0f, 0.0f,
        -0.3f, -6.0f, 0.0f,

        0.3f, -4.0f, 0.0f,
        -0.3f, -6.0f, 0.0f,
        0.3f, -6.0f, 0.0f
    };

    float triangle_color[] =
    {
        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.35f, 0.65f, 0.0f,
        0.25f, 0.45f, 0.0f,
        0.25f, 0.45f, 0.0f,

        0.42f, 0.22f, 0.0f,
        0.42f, 0.22f, 0.0f,
        0.42f, 0.22f, 0.0f,

        0.42f, 0.22f, 0.0f,
        0.42f, 0.22f, 0.0f,
        0.42f, 0.22f, 0.0f
    };

    // Code
    vkResult = __createVertexBuffer(0, triangle_position, sizeof(triangle_position), triangle_color, sizeof(triangle_color));
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s() => __createVertexBuffer() Failed For Tree Index 0, Reason : %d !!!\n", __func__, vkResult);
        return vkResult;
    }    
    else
        fprintf(gpFile, "%s() => __createVertexBuffer() Succeeded\n", __func__);

    return vkResult;
}

VkResult updateUniformBuffer_trees(void)
{
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;

    // Code
    VP_UniformData mvp_UniformData;
    memset((void*)&mvp_UniformData, 0, sizeof(VP_UniformData));

    glm::mat4 translationMatrix = glm::mat4(1.0f);

    //* Tree 1
    //* ----------------------------------------------------------------------------------------
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -12.0f));
    trees[0].modelData.modelMatrix = glm::mat4(1.0f);
    trees[0].modelData.modelMatrix = translationMatrix;
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

void buildCommandBuffers_trees(int swapchainImageIndex)
{
    // Code
    __buildCommandBuffers(swapchainImageIndex);
}

void uninitialize_trees(void)
{
    // Code
    for (int i = TREES_COUNT - 1; i >= 0; i--)
    {
        if (trees[i].vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, trees[i].vertexData_color.vkDeviceMemory, NULL);
            trees[i].vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkFreeMemory() Succeeded For Color Buffer, Tree Index : %d\n", __func__, i);
        }

        if (trees[i].vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, trees[i].vertexData_color.vkBuffer, NULL);
            trees[i].vertexData_color.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkDestroyBuffer() Succeeded For Color Buffer, Tree Index : %d\n", __func__, i);
        }

        if (trees[i].vertexData_position.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, trees[i].vertexData_position.vkDeviceMemory, NULL);
            trees[i].vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkFreeMemory() Succeeded For Position Buffer, Tree Index : %d\n", __func__, i);
        }

        if (trees[i].vertexData_position.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, trees[i].vertexData_position.vkBuffer, NULL);
            trees[i].vertexData_position.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() => vkDestroyBuffer() Succeeded For Position Buffer, Tree Index : %d\n", __func__, i);
        }
    }
}
