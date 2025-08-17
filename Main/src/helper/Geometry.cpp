#include "Geometry.h"

Pyramid::Pyramid(/* args */)
{
    // VARIABLE DECLARATIONS
    VkResult vkResult = VK_SUCCESS;

    float pyramidVertices[] = {
        // front
        0.0f, 1.0f, 0.0f,   // front-top
        -1.0f, -1.0f, 1.0f, // front-left
        1.0f, -1.0f, 1.0f,  // front-right

        // right
        0.0f, 1.0f, 0.0f,   // right-top
        1.0f, -1.0f, 1.0f,  // right-left
        1.0f, -1.0f, -1.0f, // right-right

        // back
        0.0f, 1.0f, 0.0f,    // back-top
        1.0f, -1.0f, -1.0f,  // back-left
        -1.0f, -1.0f, -1.0f, // back-right

        // left
        0.0f, 1.0f, 0.0f,    // left-top
        -1.0f, -1.0f, -1.0f, // left-left
        -1.0f, -1.0f, 1.0f,  // left-right
    };

    float pyramidColors[] = {
        // front
        1.0f, 0.0f, 0.0f, // front-top
        0.0f, 1.0f, 0.0f, // front-left
        0.0f, 0.0f, 1.0f, // front-right

        // right
        1.0f, 0.0f, 0.0f, // right-top
        0.0f, 0.0f, 1.0f, // right-left
        0.0f, 1.0f, 0.0f, // right-right

        // back
        1.0f, 0.0f, 0.0f, // back-top
        0.0f, 1.0f, 0.0f, // back-left
        0.0f, 0.0f, 1.0f, // back-right

        // left
        1.0f, 0.0f, 0.0f, // left-top
        0.0f, 0.0f, 1.0f, // left-left
        0.0f, 1.0f, 0.0f, // left-right

    };

    // CODE
    // VERTEX POSITION BUFFER
    memset((void *)&vertexData_position, 0, sizeof(VertexData));

    VkBufferCreateInfo vkBufferCreateInfo;
    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
    vkBufferCreateInfo.size = sizeof(pyramidVertices);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED.\n", __func__);
    }

    VkMemoryRequirements vkMemoryRequirements;
    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

    vkGetBufferMemoryRequirements(vkDevice, vertexData_position.vkBuffer, &vkMemoryRequirements);

    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
            if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                fprintf(gpFile, "%s => Found suitable memory type at index %u\n", __func__, i);
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED. \n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED.\n", __func__);
    }

    vkResult = vkBindBufferMemory(vkDevice, vertexData_position.vkBuffer, vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED.\n", __func__);
    }

    void *data = NULL;
    vkResult = vkMapMemory(vkDevice, vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkMapMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED.\n", __func__);
    }

    // ACTUAL MMIO
    memcpy(data, pyramidVertices, sizeof(pyramidVertices)); // THIS MEMORY IS INTEGRAL ALLIGNED

    // UNMAP
    vkUnmapMemory(vkDevice, vertexData_position.vkDeviceMemory);

    // --------------------------------------------------------------------
    // VERTEX COLOR BUFFER
    memset((void *)&vertexData_color, 0, sizeof(VertexData));

    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
    vkBufferCreateInfo.size = sizeof(pyramidColors);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

    vkGetBufferMemoryRequirements(vkDevice, vertexData_color.vkBuffer, &vkMemoryRequirements);

    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
            if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                fprintf(gpFile, "%s => Found suitable memory type at index %u FOR COLOR BUFFER\n", __func__, i);
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    vkResult = vkBindBufferMemory(vkDevice, vertexData_color.vkBuffer, vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    void *data_color = NULL;
    vkResult = vkMapMemory(vkDevice, vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data_color);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkMapMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    // ACTUAL MMIO
    memcpy(data_color, pyramidColors, sizeof(pyramidColors)); // THIS MEMORY IS INTEGRAL ALLIGNED

    // UNMAP
    vkUnmapMemory(vkDevice, vertexData_color.vkDeviceMemory);
}

Pyramid::~Pyramid()
{
    // DESTROY VERTEX BUFFER
    if (vertexData_position.vkBuffer)
    {
        vkDestroyBuffer(vkDevice, vertexData_position.vkBuffer, NULL);
        vertexData_position.vkBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => Buffer DESTROYED SUCCESSFULLY FOR POSITION BUFFER.\n", __func__);
    }

    // DESTROY DEVICE MEMORY
    if (vertexData_position.vkDeviceMemory)
    {
        vkFreeMemory(vkDevice, vertexData_position.vkDeviceMemory, NULL);
        vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => DEVICE MEMORY FREE SUCCESSFULLY FOR POSITION BUFFER.\n", __func__);
    }

    // DESTROY COLOR VERTEX BUFFER
    if (vertexData_color.vkBuffer)
    {
        vkDestroyBuffer(vkDevice, vertexData_color.vkBuffer, NULL);
        vertexData_color.vkBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => BUFFER DESTROYED SUCCESSFULLY FOR COLOR.\n", __func__);
    }

    // DESTROY COLOR DEVICE MEMORY
    if (vertexData_color.vkDeviceMemory)
    {
        vkFreeMemory(vkDevice, vertexData_color.vkDeviceMemory, NULL);
        vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => DEVICE MEMORY FREE SUCCESSFULLY FOR COLOR.\n", __func__);
    }
}

void Pyramid::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    // BIND WITH VERTEX POSITION BUFFER
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void *)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexData_position.vkBuffer, vkDeviceSize_offset_position);

    // BIND WITH VERTEX COLOR BUFFER
    VkDeviceSize vkDeviceSize_offset_color[1];
    memset((void *)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
    vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vertexData_color.vkBuffer, vkDeviceSize_offset_color);

    vkCmdDraw(commandBuffer, 12, 1, 0, 0);
}

Quad::Quad()
{
    // VARIABLE DECLARATIONS
    VkResult vkResult = VK_SUCCESS;

    float quadVertices[] = {
        // FIRST TRIANGLE
        1.0f, 1.0f, 0.0f,   // TOP-RIGHT
        -1.0f, 1.0f, 0.0f,  // TOP-LEFT
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT

        // SECOND TRIANGLE
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT
        1.0f, -1.0f, 0.0f,  // BOTTOM-RIGHT
        1.0f, 1.0f, 0.0f    // TOP-RIGHT

    };

    float quadColors[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f

    };

    // CODE
    // VERTEX POSITION BUFFER
    memset((void *)&vertexData_position, 0, sizeof(VertexData));

    VkBufferCreateInfo vkBufferCreateInfo;
    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
    vkBufferCreateInfo.size = sizeof(quadVertices);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED.\n", __func__);
    }

    VkMemoryRequirements vkMemoryRequirements;
    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

    vkGetBufferMemoryRequirements(vkDevice, vertexData_position.vkBuffer, &vkMemoryRequirements);

    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
            if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                fprintf(gpFile, "%s => Found suitable memory type at index %u\n", __func__, i);
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED. \n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED.\n", __func__);
    }

    vkResult = vkBindBufferMemory(vkDevice, vertexData_position.vkBuffer, vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED.\n", __func__);
    }

    void *data = NULL;
    vkResult = vkMapMemory(vkDevice, vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkMapMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED.\n", __func__);
    }

    // ACTUAL MMIO
    memcpy(data, quadVertices, sizeof(quadVertices)); // THIS MEMORY IS INTEGRAL ALLIGNED

    // UNMAP
    vkUnmapMemory(vkDevice, vertexData_position.vkDeviceMemory);


    // --------------------------------------------------------------------
    // VERTEX COLOR BUFFER
    memset((void *)&vertexData_color, 0, sizeof(VertexData));

    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
    vkBufferCreateInfo.size = sizeof(quadColors);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

    vkGetBufferMemoryRequirements(vkDevice, vertexData_color.vkBuffer, &vkMemoryRequirements);

    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
            if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                fprintf(gpFile, "%s => Found suitable memory type at index %u FOR COLOR BUFFER\n", __func__, i);
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    vkResult = vkBindBufferMemory(vkDevice, vertexData_color.vkBuffer, vertexData_color.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    void *data_color = NULL;
    vkResult = vkMapMemory(vkDevice, vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data_color);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkMapMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
    }

    // ACTUAL MMIO
    memcpy(data_color, quadColors, sizeof(quadColors)); // THIS MEMORY IS INTEGRAL ALLIGNED

    // UNMAP
    vkUnmapMemory(vkDevice, vertexData_color.vkDeviceMemory);
}

Quad::Quad(bool _isTextureEnabled)
{
    // VARIABLE DECLARATIONS
    VkResult vkResult = VK_SUCCESS;

    this->isTextureEnabled = _isTextureEnabled;

    float quadVertices[] = {
        // FIRST TRIANGLE
        1.0f, 1.0f, 0.0f,   // TOP-RIGHT
        -1.0f, 1.0f, 0.0f,  // TOP-LEFT
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT

        // SECOND TRIANGLE
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT
        1.0f, -1.0f, 0.0f,  // BOTTOM-RIGHT
        1.0f, 1.0f, 0.0f    // TOP-RIGHT

    };

    float quadColors[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f

    };

    float quadTexcoords[] = 
    {
        // First Triangle
        1.0f, 1.0f,  // Top Right
        0.0f, 1.0f,  // Top Left
        0.0f, 0.0f,  // Bottom Left

        // Second Triangle
        0.0f, 0.0f,  // Bottom Left
        1.0f, 0.0f,  // Bottom Right
        1.0f, 1.0f   // Top Right
    };

    // CODE
    // VERTEX POSITION BUFFER
    memset((void *)&vertexData_position, 0, sizeof(VertexData));

    VkBufferCreateInfo vkBufferCreateInfo;
    memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo.pNext = NULL;
    vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
    vkBufferCreateInfo.size = sizeof(quadVertices);
    vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position.vkBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED.\n", __func__);
    }

    VkMemoryRequirements vkMemoryRequirements;
    memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

    vkGetBufferMemoryRequirements(vkDevice, vertexData_position.vkBuffer, &vkMemoryRequirements);

    VkMemoryAllocateInfo vkMemoryAllocateInfo;
    memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.pNext = NULL;
    vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
        {
            // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
            if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                fprintf(gpFile, "%s => Found suitable memory type at index %u\n", __func__, i);
                vkMemoryAllocateInfo.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position.vkDeviceMemory);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED. \n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED.\n", __func__);
    }

    vkResult = vkBindBufferMemory(vkDevice, vertexData_position.vkBuffer, vertexData_position.vkDeviceMemory, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED.\n", __func__);
    }

    void *data = NULL;
    vkResult = vkMapMemory(vkDevice, vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkMapMemory() IS FAILED.\n", __func__);
        // return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED.\n", __func__);
    }

    // ACTUAL MMIO
    memcpy(data, quadVertices, sizeof(quadVertices)); // THIS MEMORY IS INTEGRAL ALLIGNED

    // UNMAP
    vkUnmapMemory(vkDevice, vertexData_position.vkDeviceMemory);

    if (this->isTextureEnabled)
    {
        // --------------------------------------------------------------------
        // VERTEX TEXCOORD BUFFER
        //* Step - 4
        memset((void*)&vertexData_texcoord, 0, sizeof(VertexData));

        //* Step - 5
        memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));
        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.flags = 0;   //! Valid Flags are used in sparse(scattered) buffers
        vkBufferCreateInfo.pNext = NULL;
        vkBufferCreateInfo.size = sizeof(quadTexcoords);
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        
        //* Step - 6
        vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_texcoord.vkBuffer);
        if (vkResult != VK_SUCCESS)
            fprintf(gpFile, "%s() => vkCreateBuffer() Failed For Vertex Texture Buffer : %d !!!\n", __func__, vkResult);
        else
            fprintf(gpFile, "%s() => vkCreateBuffer() Succeeded For Vertex Texture Buffer\n", __func__);
        
        //* Step - 7
        memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
        vkGetBufferMemoryRequirements(vkDevice, vertexData_texcoord.vkBuffer, &vkMemoryRequirements);

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
        vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_texcoord.vkDeviceMemory);
        if (vkResult != VK_SUCCESS)
            fprintf(gpFile, "%s() => vkAllocateMemory() Failed For Vertex Texture Buffer : %d !!!\n", __func__, vkResult);
        else
            fprintf(gpFile, "%s() => vkAllocateMemory() Succeeded For Vertex Texture Buffer\n", __func__);

        //* Step - 10
        //! Binds Vulkan Device Memory Object Handle with the Vulkan Buffer Object Handle
        vkResult = vkBindBufferMemory(vkDevice, vertexData_texcoord.vkBuffer, vertexData_texcoord.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
            fprintf(gpFile, "%s() => vkBindBufferMemory() Failed For Vertex Texture Buffer : %d !!!\n", __func__, vkResult);
        else
            fprintf(gpFile, "%s() => vkBindBufferMemory() Succeeded For Vertex Texture Buffer\n", __func__);

        //* Step - 11
        data = NULL;
        vkResult = vkMapMemory(vkDevice, vertexData_texcoord.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
        if (vkResult != VK_SUCCESS)
            fprintf(gpFile, "%s() => vkMapMemory() Failed For Vertex Texture Buffer : %d !!!\n", __func__, vkResult);
        else
            fprintf(gpFile, "%s() => vkMapMemory() Succeeded For Vertex Texture Buffer\n", __func__);

        //* Step - 12
        memcpy(data, quadTexcoords, sizeof(quadTexcoords));

        //* Step - 13
        vkUnmapMemory(vkDevice, vertexData_texcoord.vkDeviceMemory);
    }
    else
    {   
        // --------------------------------------------------------------------
        // VERTEX COLOR BUFFER
        memset((void *)&vertexData_color, 0, sizeof(VertexData));

        memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.pNext = NULL;
        vkBufferCreateInfo.flags = 0; // VALID FLAGS ARE USED IN SCATTERED/SPARSE BUFFERS
        vkBufferCreateInfo.size = sizeof(quadColors);
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color.vkBuffer);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s => vkCreateBuffer() IS FAILED FOR COLOR BUFFER.\n", __func__);
            // return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s => vkCreateBuffer() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
        }

        memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

        vkGetBufferMemoryRequirements(vkDevice, vertexData_color.vkBuffer, &vkMemoryRequirements);

        memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

        vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vkMemoryAllocateInfo.pNext = NULL;
        vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
        vkMemoryAllocateInfo.memoryTypeIndex = 0; // INITAILZIE VALUE BEFORE ENTERING INTO LOOP

        for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
            {
                // if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) // I HAVE UPDATED WITH BELOW LINE BEACUSE ITS FAILING FOR MY GPU
                if ((vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
                {
                    fprintf(gpFile, "%s => Found suitable memory type at index %u FOR COLOR BUFFER\n", __func__, i);
                    vkMemoryAllocateInfo.memoryTypeIndex = i;
                    break;
                }
            }
            vkMemoryRequirements.memoryTypeBits >>= 1;
        }

        vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color.vkDeviceMemory);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s => vkAllocateMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
            // return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s => vkAllocateMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
        }

        vkResult = vkBindBufferMemory(vkDevice, vertexData_color.vkBuffer, vertexData_color.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s => vkBindBufferMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
            // return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s => vkBindBufferMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
        }

        void *data_color = NULL;
        vkResult = vkMapMemory(vkDevice, vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data_color);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s => vkMapMemory() IS FAILED FOR COLOR BUFFER.\n", __func__);
            // return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s => vkMapMemory() IS SUCCEEDED FOR COLOR BUFFER.\n", __func__);
        }

        // ACTUAL MMIO
        memcpy(data_color, quadColors, sizeof(quadColors)); // THIS MEMORY IS INTEGRAL ALLIGNED

        // UNMAP
        vkUnmapMemory(vkDevice, vertexData_color.vkDeviceMemory);
    }
}

Quad::~Quad()
{
    if (this->isTextureEnabled)
    {
        // DESTROY TEXCOORD VERTEX BUFFER
        if (vertexData_texcoord.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, vertexData_texcoord.vkBuffer, NULL);
            vertexData_texcoord.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s => BUFFER DESTROYED SUCCESSFULLY FOR TEXCOORD.\n", __func__);
        }

        // DESTROY TEXCOORD DEVICE MEMORY
        if (vertexData_texcoord.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, vertexData_texcoord.vkDeviceMemory, NULL);
            vertexData_texcoord.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s => DEVICE MEMORY FREED SUCCESSFULLY FOR TEXCOORD.\n", __func__);
        }
    }
    else
    {
        // DESTROY COLOR VERTEX BUFFER
        if (vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, vertexData_color.vkBuffer, NULL);
            vertexData_color.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s => BUFFER DESTROYED SUCCESSFULLY FOR COLOR.\n", __func__);
        }

        // DESTROY COLOR DEVICE MEMORY
        if (vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, vertexData_color.vkDeviceMemory, NULL);
            vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s => DEVICE MEMORY FREE SUCCESSFULLY FOR COLOR.\n", __func__);
        }
    }
    
    // DESTROY VERTEX BUFFER
    if (vertexData_position.vkBuffer)
    {
        vkDestroyBuffer(vkDevice, vertexData_position.vkBuffer, NULL);
        vertexData_position.vkBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => Buffer DESTROYED SUCCESSFULLY FOR POSITION BUFFER.\n", __func__);
    }

    // DESTROY DEVICE MEMORY
    if (vertexData_position.vkDeviceMemory)
    {
        vkFreeMemory(vkDevice, vertexData_position.vkDeviceMemory, NULL);
        vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => DEVICE MEMORY FREE SUCCESSFULLY FOR POSITION BUFFER.\n", __func__);
    }
}

void Quad::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    // BIND WITH VERTEX POSITION BUFFER
    VkDeviceSize vkDeviceSize_offset_position[1];
    memset((void *)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexData_position.vkBuffer, vkDeviceSize_offset_position);

    if (this->isTextureEnabled)
    {
        //! Bind with Vertex Texture Buffer
        VkDeviceSize vkDeviceSize_offset_texture[1];
        memset((void*)vkDeviceSize_offset_texture, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_texture));
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vertexData_texcoord.vkBuffer, vkDeviceSize_offset_texture);
    }
    else
    {
        // BIND WITH VERTEX COLOR BUFFER
        VkDeviceSize vkDeviceSize_offset_color[1];
        memset((void *)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vertexData_color.vkBuffer, vkDeviceSize_offset_color);
    }

    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

