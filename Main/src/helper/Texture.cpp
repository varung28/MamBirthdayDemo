#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern VkCommandPool vkCommandPool;

Texture::Texture()
{
    vkImage_texture = VK_NULL_HANDLE;
    vkDeviceMemory_texture = VK_NULL_HANDLE;
    vkImageView_texture = VK_NULL_HANDLE;
    vkSampler_texture = VK_NULL_HANDLE;
}

VkResult Texture::createTexture(std::string filePath)
{
    // Code
    // Variable Declarations
    VkResult vkResult = VK_SUCCESS;
    uint8_t* imageData = NULL;
    int imageWidth = 0, imageHeight = 0, numChannels = 0;

    VkBuffer vkBuffer_stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
    VkDeviceSize imageSize;

    // Code
    const char* textureFileName = filePath.c_str();

    //! Step - 1
    stbi_set_flip_vertically_on_load(true); 
    
    imageData = stbi_load(textureFileName, &imageWidth, &imageHeight, &numChannels, STBI_rgb_alpha);
    if (imageData == NULL || imageWidth <= 0 || imageHeight <= 0 || numChannels <= 0)
    {
        fprintf(gpFile, "TEXTURE::%s() => stbi_load() Failed For %s !!!\n", __func__, textureFileName);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }
   
    imageSize = imageWidth * imageHeight * 4;

    fprintf(gpFile, "\n%s Properties\n", textureFileName);
    fprintf(gpFile, "-------------------------------------------\n");
    fprintf(gpFile, "Image Width = %d\n", imageWidth);
    fprintf(gpFile, "Image Height = %d\n", imageHeight);
    fprintf(gpFile, "Image Size = %lld\n", imageSize);
    fprintf(gpFile, "-------------------------------------------\n\n");   

    //! Step - 2
    VkBufferCreateInfo vkBufferCreateInfo_stagingBuffer;
    memset((void*)&vkBufferCreateInfo_stagingBuffer, 0, sizeof(VkBufferCreateInfo));
    vkBufferCreateInfo_stagingBuffer.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vkBufferCreateInfo_stagingBuffer.pNext = NULL;
    vkBufferCreateInfo_stagingBuffer.flags = 0;
    vkBufferCreateInfo_stagingBuffer.size = imageSize;
    vkBufferCreateInfo_stagingBuffer.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkBufferCreateInfo_stagingBuffer.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;  //* This denotes source data to be transferred to VkImage

    vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo_stagingBuffer, NULL, &vkBuffer_stagingBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkCreateBuffer() Failed For Staging Buffer : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (imageData)
        {
            stbi_image_free(imageData);
            imageData = NULL;
            fprintf(gpFile, "TEXTURE::%s() => stbi_image_free() Called For Texture : %s\n", __func__, textureFileName);
        }

        return vkResult;
    }
        
    else
        fprintf(gpFile, "TEXTURE::%s() => vkCreateBuffer() Succeeded For Staging Buffer : %s\n", __func__, textureFileName);

    VkMemoryRequirements vkMemoryRequirements_stagingBuffer;
    memset((void*)&vkMemoryRequirements_stagingBuffer, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(vkDevice, vkBuffer_stagingBuffer, &vkMemoryRequirements_stagingBuffer);

    VkMemoryAllocateInfo vkMemoryAllocateInfo_stagingBuffer;
    memset((void*)&vkMemoryAllocateInfo_stagingBuffer, 0, sizeof(VkMemoryAllocateInfo));
    vkMemoryAllocateInfo_stagingBuffer.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo_stagingBuffer.pNext = NULL;
    vkMemoryAllocateInfo_stagingBuffer.allocationSize = vkMemoryRequirements_stagingBuffer.size;
    vkMemoryAllocateInfo_stagingBuffer.memoryTypeIndex = 0;

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements_stagingBuffer.memoryTypeBits & 1) == 1)
        {
            if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
            {
                vkMemoryAllocateInfo_stagingBuffer.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements_stagingBuffer.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo_stagingBuffer, NULL, &vkDeviceMemory_stagingBuffer);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateMemory() Failed For Staging Buffer : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }
        if (imageData)
        {
            stbi_image_free(imageData);
            imageData = NULL;
            fprintf(gpFile, "TEXTURE::%s() => stbi_image_free() Called For Texture : %s\n", __func__, textureFileName);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateMemory() Succeeded For Staging Buffer : %s\n", __func__, textureFileName);

    vkResult = vkBindBufferMemory(vkDevice, vkBuffer_stagingBuffer, vkDeviceMemory_stagingBuffer, 0);
    if (vkResult != VK_SUCCESS)
    { 
        fprintf(gpFile, "TEXTURE::%s() => vkBindBufferMemory() Failed For Staging Buffer : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }
        if (imageData)
        {
            stbi_image_free(imageData);
            imageData = NULL;
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkBindBufferMemory() Succeeded For Staging Buffer : %s\n", __func__, textureFileName);

    void* data = NULL;
    vkResult = vkMapMemory(
        vkDevice, 
        vkDeviceMemory_stagingBuffer,
        0, 
        imageSize, 
        0, 
        &data
    );
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkMapMemory() Failed For Staging Buffer : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }
        if (imageData)
        {
            stbi_image_free(imageData);
            imageData = NULL;
            fprintf(gpFile, "TEXTURE::%s() => stbi_image_free() Called For Texture : %s\n", __func__, textureFileName);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkMapMemory() Succeeded For Staging Buffer : %s\n", __func__, textureFileName);

    memcpy(data, imageData, imageSize);

    vkUnmapMemory(vkDevice, vkDeviceMemory_stagingBuffer);

    //* Free the image data given by stb, as it is copied in image staging buffer
    stbi_image_free(imageData);
    imageData = NULL;
    fprintf(gpFile, "TEXTURE::%s() => stbi_image_free() Called For Texture : %s\n", __func__, textureFileName);

    //! Step - 3
    VkImageCreateInfo vkImageCreateInfo;
    memset((void*)&vkImageCreateInfo, 0, sizeof(VkImageCreateInfo));
    vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vkImageCreateInfo.flags = 0;
    vkImageCreateInfo.pNext = NULL;
    vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    vkImageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    vkImageCreateInfo.extent.width = imageWidth;
    vkImageCreateInfo.extent.height = imageHeight;
    vkImageCreateInfo.extent.depth = 1;
    vkImageCreateInfo.mipLevels = 1;
    vkImageCreateInfo.arrayLayers = 1;
    vkImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    vkImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    vkImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    vkImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    vkResult = vkCreateImage(vkDevice, &vkImageCreateInfo, NULL, &vkImage_texture);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkCreateImage() Failed For Texture : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }     
    else
        fprintf(gpFile, "TEXTURE::%s() => vkCreateImage() Succeeded For Texture : %s\n", __func__, textureFileName);

    VkMemoryRequirements vkMemoryRequirements_image;
    memset((void*)&vkMemoryRequirements_image, 0, sizeof(VkMemoryRequirements));
    vkGetImageMemoryRequirements(vkDevice, vkImage_texture, &vkMemoryRequirements_image);

    VkMemoryAllocateInfo vkMemoryAllocateInfo_image;
    memset((void*)&vkMemoryAllocateInfo_image, 0, sizeof(VkMemoryAllocateInfo));
    vkMemoryAllocateInfo_image.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo_image.pNext = NULL;
    vkMemoryAllocateInfo_image.memoryTypeIndex = 0;
    vkMemoryAllocateInfo_image.allocationSize = vkMemoryRequirements_image.size;

    for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((vkMemoryRequirements_image.memoryTypeBits & 1) == 1)
        {
            if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
            {
                vkMemoryAllocateInfo_image.memoryTypeIndex = i;
                break;
            }
        }
        vkMemoryRequirements_image.memoryTypeBits >>= 1;
    }

    vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo_image, NULL, &vkDeviceMemory_texture);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateMemory() Failed For Texture : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateMemory() Succeeded For Texture : %s\n", __func__, textureFileName);

    vkResult = vkBindImageMemory(vkDevice, vkImage_texture, vkDeviceMemory_texture, 0);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkBindImageMemory() Failed For Texture : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }
        if (imageData)
        {
            stbi_image_free(imageData);
            imageData = NULL;
            fprintf(gpFile, "TEXTURE::%s() => stbi_image_free() Called For Texture : %s\n", __func__, textureFileName);
        }

        return vkResult;
    }      
    else
        fprintf(gpFile, "TEXTURE::%s() => vkBindImageMemory() Succeeded For Texture : %s\n", __func__, textureFileName);

    //! Step - 4
    //! ----------------------------------------------------------------------------------------------------------------------------------------------------------

    //* Step - 4.1
    VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo_transition_image_layout;
    memset((void*)&vkCommandBufferAllocateInfo_transition_image_layout, 0, sizeof(VkCommandBufferAllocateInfo));
    vkCommandBufferAllocateInfo_transition_image_layout.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    vkCommandBufferAllocateInfo_transition_image_layout.pNext = NULL;
    vkCommandBufferAllocateInfo_transition_image_layout.commandPool = vkCommandPool;
    vkCommandBufferAllocateInfo_transition_image_layout.commandBufferCount = 1;
    vkCommandBufferAllocateInfo_transition_image_layout.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
    vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo_transition_image_layout, &vkCommandBuffer_transition_image_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Failed For vkCommandBuffer_transition_image_layout : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }   
    else
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 4.2
    VkCommandBufferBeginInfo vkCommandBufferBeginInfo_image_transition_layout;
    memset((void*)&vkCommandBufferBeginInfo_image_transition_layout, 0, sizeof(VkCommandBufferBeginInfo));
    vkCommandBufferBeginInfo_image_transition_layout.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkCommandBufferBeginInfo_image_transition_layout.pNext = NULL;
    vkCommandBufferBeginInfo_image_transition_layout.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkResult = vkBeginCommandBuffer(vkCommandBuffer_transition_image_layout, &vkCommandBufferBeginInfo_image_transition_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkBeginCommandBuffer() Failed For vkCommandBuffer_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkBeginCommandBuffer() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 4.3
    VkPipelineStageFlags vkPipelineStageFlags_source = 0;
    VkPipelineStageFlags vkPipelineStageFlags_destination = 0;

    VkImageMemoryBarrier vkImageMemoryBarrier;
    memset((void*)&vkImageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    vkImageMemoryBarrier.pNext = NULL;
    vkImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    vkImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.image = vkImage_texture;
    vkImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vkImageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    vkImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    vkImageMemoryBarrier.subresourceRange.layerCount = 1;
    vkImageMemoryBarrier.subresourceRange.levelCount = 1;

    if (vkImageMemoryBarrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && vkImageMemoryBarrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        vkImageMemoryBarrier.srcAccessMask = 0;
        vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkPipelineStageFlags_source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        vkPipelineStageFlags_destination = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (vkImageMemoryBarrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && vkImageMemoryBarrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkPipelineStageFlags_source = VK_PIPELINE_STAGE_TRANSFER_BIT;
        vkPipelineStageFlags_destination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => Unsupported Texture Layout Transition !!!\n", __func__);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }

    vkCmdPipelineBarrier(
        vkCommandBuffer_transition_image_layout,
        vkPipelineStageFlags_source,
        vkPipelineStageFlags_destination,
        0,
        0,
        NULL,
        0,
        NULL,
        1,
        &vkImageMemoryBarrier
    );

    //* Step - 4.4
    vkResult = vkEndCommandBuffer(vkCommandBuffer_transition_image_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkEndCommandBuffer() Failed For vkCommandBuffer_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkEndCommandBuffer() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 4.5
    VkSubmitInfo vkSubmitInfo_transition_image_layout;
    memset((void*)&vkSubmitInfo_transition_image_layout, 0, sizeof(VkSubmitInfo));
    vkSubmitInfo_transition_image_layout.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkSubmitInfo_transition_image_layout.pNext = NULL;
    vkSubmitInfo_transition_image_layout.commandBufferCount = 1;
    vkSubmitInfo_transition_image_layout.pCommandBuffers = &vkCommandBuffer_transition_image_layout;

    vkResult = vkQueueSubmit(vkQueue, 1, &vkSubmitInfo_transition_image_layout, VK_NULL_HANDLE);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueSubmit() Failed For vkSubmitInfo_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueSubmit() Succeeded For vkSubmitInfo_transition_image_layout\n", __func__);

    //* Step - 4.6
    vkResult = vkQueueWaitIdle(vkQueue);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueWaitIdle() Failed For vkSubmitInfo_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueWaitIdle() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 4.7
    if (vkCommandBuffer_transition_image_layout)
    {
        vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
        vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
    }
    //! ----------------------------------------------------------------------------------------------------------------------------------------------------------
    
    //! Step - 5
    VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo_buffer_to_image_copy;
    memset((void*)&vkCommandBufferAllocateInfo_buffer_to_image_copy, 0, sizeof(VkCommandBufferAllocateInfo));
    vkCommandBufferAllocateInfo_buffer_to_image_copy.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    vkCommandBufferAllocateInfo_buffer_to_image_copy.pNext = NULL;
    vkCommandBufferAllocateInfo_buffer_to_image_copy.commandPool = vkCommandPool;
    vkCommandBufferAllocateInfo_buffer_to_image_copy.commandBufferCount = 1;
    vkCommandBufferAllocateInfo_buffer_to_image_copy.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
    vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo_buffer_to_image_copy, &vkCommandBuffer_buffer_to_image_copy);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Failed For vkCommandBuffer_buffer_to_image_copy : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }    
    else
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);

    VkCommandBufferBeginInfo vkCommandBufferBeginInfo_buffer_to_image_copy;
    memset((void*)&vkCommandBufferBeginInfo_buffer_to_image_copy, 0, sizeof(VkCommandBufferBeginInfo));
    vkCommandBufferBeginInfo_buffer_to_image_copy.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkCommandBufferBeginInfo_buffer_to_image_copy.pNext = NULL;
    vkCommandBufferBeginInfo_buffer_to_image_copy.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkResult = vkBeginCommandBuffer(vkCommandBuffer_buffer_to_image_copy, &vkCommandBufferBeginInfo_buffer_to_image_copy);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkBeginCommandBuffer() Failed For vkCommandBuffer_buffer_to_image_copy : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_buffer_to_image_copy)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_buffer_to_image_copy);
            vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
            fprintf(gpFile, "ERROR : TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkBeginCommandBuffer() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);

    VkBufferImageCopy vkBufferImageCopy;
    memset((void*)&vkBufferImageCopy, 0, sizeof(VkBufferImageCopy));
    vkBufferImageCopy.bufferOffset = 0;
    vkBufferImageCopy.bufferRowLength = 0;
    vkBufferImageCopy.bufferImageHeight = 0;
    vkBufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vkBufferImageCopy.imageSubresource.mipLevel = 0;
    vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
    vkBufferImageCopy.imageSubresource.layerCount = 1;
    vkBufferImageCopy.imageOffset.x = 0; 
    vkBufferImageCopy.imageOffset.y = 0;
    vkBufferImageCopy.imageOffset.z = 0;
    vkBufferImageCopy.imageExtent.width = imageWidth;
    vkBufferImageCopy.imageExtent.height = imageHeight;
    vkBufferImageCopy.imageExtent.depth = 1;

    vkCmdCopyBufferToImage(
        vkCommandBuffer_buffer_to_image_copy,
        vkBuffer_stagingBuffer,
        vkImage_texture,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &vkBufferImageCopy
    );

    vkResult = vkEndCommandBuffer(vkCommandBuffer_buffer_to_image_copy);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkEndCommandBuffer() Failed For vkCommandBuffer_buffer_to_image_copy : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_buffer_to_image_copy)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_buffer_to_image_copy);
            vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);
        }
        if (vkImage_texture)
        {
            vkDestroyImage(vkDevice, vkImage_texture, NULL);
            vkImage_texture = NULL;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyImage() Succeeded For vkImage_texture\n", __func__);
        }
        if (vkDeviceMemory_texture)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_texture, NULL);
            vkDeviceMemory_texture = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_texture\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkEndCommandBuffer() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);

    VkSubmitInfo vkSubmitInfo_buffer_to_copy;
    memset((void*)&vkSubmitInfo_buffer_to_copy, 0, sizeof(VkSubmitInfo));
    vkSubmitInfo_buffer_to_copy.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkSubmitInfo_buffer_to_copy.pNext = NULL;
    vkSubmitInfo_buffer_to_copy.commandBufferCount = 1;
    vkSubmitInfo_buffer_to_copy.pCommandBuffers = &vkCommandBuffer_buffer_to_image_copy;

    vkResult = vkQueueSubmit(vkQueue, 1, &vkSubmitInfo_buffer_to_copy, VK_NULL_HANDLE);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueSubmit() Failed For vkSubmitInfo_buffer_to_copy : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_buffer_to_image_copy)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_buffer_to_image_copy);
            vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
            fprintf(gpFile, "ERROR : TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueSubmit() Succeeded For vkSubmitInfo_buffer_to_copy\n", __func__);

    vkResult = vkQueueWaitIdle(vkQueue);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueWaitIdle() Failed For vkCommandBuffer_buffer_to_image_copy : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_buffer_to_image_copy)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_buffer_to_image_copy);
            vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
            fprintf(gpFile, "ERROR : TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueWaitIdle() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);

    if (vkCommandBuffer_buffer_to_image_copy)
    {
        vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_buffer_to_image_copy);
        vkCommandBuffer_buffer_to_image_copy = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_buffer_to_image_copy\n", __func__);
    }    

    //! Step - 6
    //! ----------------------------------------------------------------------------------------------------------------------------------------------------------

    //* Step - 6.1
    memset((void*)&vkCommandBufferAllocateInfo_transition_image_layout, 0, sizeof(VkCommandBufferAllocateInfo));
    vkCommandBufferAllocateInfo_transition_image_layout.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    vkCommandBufferAllocateInfo_transition_image_layout.pNext = NULL;
    vkCommandBufferAllocateInfo_transition_image_layout.commandPool = vkCommandPool;
    vkCommandBufferAllocateInfo_transition_image_layout.commandBufferCount = 1;
    vkCommandBufferAllocateInfo_transition_image_layout.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
    vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo_transition_image_layout, &vkCommandBuffer_transition_image_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Failed For vkCommandBuffer_transition_image_layout : %d !!!\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }  
    else
        fprintf(gpFile, "TEXTURE::%s() => vkAllocateCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 6.2
    memset((void*)&vkCommandBufferBeginInfo_image_transition_layout, 0, sizeof(VkCommandBufferBeginInfo));
    vkCommandBufferBeginInfo_image_transition_layout.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkCommandBufferBeginInfo_image_transition_layout.pNext = NULL;
    vkCommandBufferBeginInfo_image_transition_layout.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkResult = vkBeginCommandBuffer(vkCommandBuffer_transition_image_layout, &vkCommandBufferBeginInfo_image_transition_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkBeginCommandBuffer() Failed For vkCommandBuffer_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkBeginCommandBuffer() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 6.3
    vkPipelineStageFlags_source = 0;
    vkPipelineStageFlags_destination = 0;

    memset((void*)&vkImageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));
    vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    vkImageMemoryBarrier.pNext = NULL;
    vkImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vkImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vkImageMemoryBarrier.image = vkImage_texture;
    vkImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vkImageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    vkImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    vkImageMemoryBarrier.subresourceRange.layerCount = 1;
    vkImageMemoryBarrier.subresourceRange.levelCount = 1;

    if (vkImageMemoryBarrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && vkImageMemoryBarrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        vkImageMemoryBarrier.srcAccessMask = 0;
        vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkPipelineStageFlags_source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        vkPipelineStageFlags_destination = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (vkImageMemoryBarrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && vkImageMemoryBarrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkPipelineStageFlags_source = VK_PIPELINE_STAGE_TRANSFER_BIT;
        vkPipelineStageFlags_destination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => Unsupported Texture Layout Transition !!!\n", __func__);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;

        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }

    vkCmdPipelineBarrier(
        vkCommandBuffer_transition_image_layout,
        vkPipelineStageFlags_source,
        vkPipelineStageFlags_destination,
        0,
        0,
        NULL,
        0,
        NULL,
        1,
        &vkImageMemoryBarrier
    );

    //* Step - 6.4
    vkResult = vkEndCommandBuffer(vkCommandBuffer_transition_image_layout);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkEndCommandBuffer() Failed For vkCommandBuffer_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkEndCommandBuffer() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 6.5
    memset((void*)&vkSubmitInfo_transition_image_layout, 0, sizeof(VkSubmitInfo));
    vkSubmitInfo_transition_image_layout.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkSubmitInfo_transition_image_layout.pNext = NULL;
    vkSubmitInfo_transition_image_layout.commandBufferCount = 1;
    vkSubmitInfo_transition_image_layout.pCommandBuffers = &vkCommandBuffer_transition_image_layout;

    vkResult = vkQueueSubmit(vkQueue, 1, &vkSubmitInfo_transition_image_layout, VK_NULL_HANDLE);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueSubmit() Failed For vkSubmitInfo_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueSubmit() Succeeded For vkSubmitInfo_transition_image_layout\n", __func__);

    //* Step - 6.6
    vkResult = vkQueueWaitIdle(vkQueue);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "ERROR : TEXTURE::%s() => vkQueueWaitIdle() Failed For vkSubmitInfo_transition_image_layout : %d\n", __func__, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        
        //* Cleanup Code
        if (vkCommandBuffer_transition_image_layout)
        {
            vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
            vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
        }
        if (vkDeviceMemory_stagingBuffer)
        {
            vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
            vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
        }
        if (vkBuffer_stagingBuffer)
        {
            vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
            vkBuffer_stagingBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
        }

        return vkResult;
    }
    else
        fprintf(gpFile, "TEXTURE::%s() => vkQueueWaitIdle() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);

    //* Step - 6.7
    if (vkCommandBuffer_transition_image_layout)
    {
        vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_transition_image_layout);
        vkCommandBuffer_transition_image_layout = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkFreeCommandBuffers() Succeeded For vkCommandBuffer_transition_image_layout\n", __func__);
    }
    //! ----------------------------------------------------------------------------------------------------------------------------------------------------------
    
    //! Step - 7
    if (vkBuffer_stagingBuffer)
    {
        vkDestroyBuffer(vkDevice, vkBuffer_stagingBuffer, NULL);
        vkBuffer_stagingBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkDestroyBuffer() Succeeded For vkBuffer_stagingBuffer\n", __func__);
    }

    if (vkDeviceMemory_stagingBuffer)
    {
        vkFreeMemory(vkDevice, vkDeviceMemory_stagingBuffer, NULL);
        vkDeviceMemory_stagingBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_stagingBuffer\n", __func__);
    }

    //! Step - 8
    VkImageViewCreateInfo vkImageViewCreateInfo;
    memset((void*)&vkImageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));
    vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vkImageViewCreateInfo.pNext = NULL;
    vkImageViewCreateInfo.flags = 0;
    vkImageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    vkImageViewCreateInfo.subresourceRange.levelCount = 1;
    vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    vkImageViewCreateInfo.subresourceRange.layerCount = 1;
    vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vkImageViewCreateInfo.image = vkImage_texture;

    vkResult = vkCreateImageView(vkDevice, &vkImageViewCreateInfo, NULL, &vkImageView_texture);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkCreateImageView() Failed For Texture : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }   
    else
        fprintf(gpFile, "TEXTURE::%s() => vkCreateImageView() Succeeded For Texture : %s\n", __func__, textureFileName);

    //! Step - 9
    VkSamplerCreateInfo vkSamplerCreateInfo;
    memset((void*)&vkSamplerCreateInfo, 0, sizeof(VkSamplerCreateInfo));
    vkSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    vkSamplerCreateInfo.pNext = NULL;
    vkSamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    vkSamplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    vkSamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    vkSamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    vkSamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    vkSamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    vkSamplerCreateInfo.anisotropyEnable = VK_FALSE;
    vkSamplerCreateInfo.maxAnisotropy = 16;
    vkSamplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    vkSamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    vkSamplerCreateInfo.compareEnable = VK_FALSE;
    vkSamplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    vkResult = vkCreateSampler(vkDevice, &vkSamplerCreateInfo, NULL, &vkSampler_texture);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "TEXTURE::%s() => vkCreateSampler() Failed For Texture : %s, Error Code : %d !!!\n", __func__, textureFileName, vkResult);
        vkResult = VK_ERROR_INITIALIZATION_FAILED;
        return vkResult;
    }    
    else
        fprintf(gpFile, "TEXTURE::%s() => vkCreateSampler() Succeeded For Texture : %s\n", __func__, textureFileName);

    return vkResult;
}

VkImageView Texture::getImageView()
{
    return this->vkImageView_texture;
}

VkSampler Texture::getSampler()
{
    return this->vkSampler_texture;
}

Texture::~Texture()
{
    if (vkSampler_texture)
    {
        vkDestroySampler(vkDevice, vkSampler_texture, NULL);
        vkSampler_texture = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkDestroySampler() Succeeded For vkSampler_texture\n", __func__);
    }

    if (vkImageView_texture)
    {
        vkDestroyImageView(vkDevice, vkImageView_texture, NULL);
        vkImageView_texture = NULL;
        fprintf(gpFile, "TEXTURE::%s() => vkDestroyImageView() Succeeded For vkImage_texture\n", __func__);
    }

    if (vkDeviceMemory_texture)
    {
        vkFreeMemory(vkDevice, vkDeviceMemory_texture, NULL);
        vkDeviceMemory_texture = VK_NULL_HANDLE;
        fprintf(gpFile, "TEXTURE::%s() => vkFreeMemory() Succeeded For vkDeviceMemory_texture\n", __func__);
    }

    if (vkImage_texture)
    {
        vkDestroyImage(vkDevice, vkImage_texture, NULL);
        vkImage_texture = NULL;
        fprintf(gpFile, "TEXTURE::%s() => vkDestroyImage() Succeeded For vkImage_texture\n", __func__);
    }

}
