#include "FadeInFadeOut.h"

Quad *FadeInFadeOut::quad{nullptr};
VkShaderModule FadeInFadeOut::vertexShaderModule{VK_NULL_HANDLE};
VkShaderModule FadeInFadeOut::fragmentShaderModule{VK_NULL_HANDLE};
VkPipeline FadeInFadeOut::vkPipeline{VK_NULL_HANDLE};
VkDescriptorSetLayout FadeInFadeOut::vkDescriptorSetLayout{VK_NULL_HANDLE};
VkDescriptorSet FadeInFadeOut::vkDescriptorSet{VK_NULL_HANDLE};
VulkanPipelineBuilder *FadeInFadeOut::pipelineBuilder{nullptr};
VkPipelineLayout FadeInFadeOut::vkPipelineLayout{VK_NULL_HANDLE};
struct FadeInFadeOut::UniformData FadeInFadeOut::uniformData;

FadeInFadeOut::FadeInFadeOut()
{
    // variable declarations

    VkResult vkResult{VK_SUCCESS};

    const std::array<float, 18> vertices = {
        // FIRST TRIANGLE
        1.0f, 1.0f, 0.0f,   // TOP-RIGHT
        -1.0f, 1.0f, 0.0f,  // TOP-LEFT
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT

        // SECOND TRIANGLE
        -1.0f, -1.0f, 0.0f, // BOTTOM-LEFT
        1.0f, -1.0f, 0.0f,  // BOTTOM-RIGHT
        1.0f, 1.0f, 0.0f    // TOP-RIGHT

    };

    const std::array<float, 24> colors = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    // code
    printf("quad == nullptr\n");
    if (quad != nullptr)
        return;

    quad = new Quad(vertices, colors);

    vertexShaderModule = ShaderModuleHelper::LoadShaderModule("bin\\fadein_fadeout.vert.spv");
    fragmentShaderModule = ShaderModuleHelper::LoadShaderModule("bin\\fadein_fadeout.frag.spv");
    pipelineBuilder = new VulkanPipelineBuilder();

    if (VK_SUCCESS != createUniformBuffer())
    {
        fprintf(gpFile, "[%s:%d] createUniformBuffer() failed.\n", __FILE__, __LINE__);
        return;
    }

    if (VK_SUCCESS != createDescriptorSetLayout())
    {
        fprintf(gpFile, "[%s:%d] createDescriptorSetLayout() failed.\n", __FILE__, __LINE__);
        return;
    }

    if (VK_SUCCESS != createPipelineLayout())
    {
        fprintf(gpFile, "[%s:%d] createPipelineLayout() failed.\n", __FILE__, __LINE__);
        return;
    }

    if (VK_SUCCESS != createDescriptorSet())
    {
        fprintf(gpFile, "[%s:%d] createDescriptorSet() failed.\n", __FILE__, __LINE__);
        return;
    }

    printf("Before createPipeline \n");
    createPipeline();
    printf("after createPipeline \n");
}

FadeInFadeOut::~FadeInFadeOut()
{
    if (quad)
    {
        delete quad;
        quad = nullptr;
    }

    if (pipelineBuilder)
    {
        delete pipelineBuilder;
        pipelineBuilder = nullptr;
    }

    if (vkDescriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, NULL);
        vkDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if (vkPipeline)
    {
        vkDestroyPipeline(vkDevice, vkPipeline, NULL);
        vkPipeline = VK_NULL_HANDLE;
    }

    if (vkPipelineLayout)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
        vkPipelineLayout = VK_NULL_HANDLE;
    }

    if (uniformData.vkBuffer)
    {
        vkDestroyBuffer(vkDevice, uniformData.vkBuffer, NULL);
        uniformData.vkBuffer = VK_NULL_HANDLE;
        fprintf(gpFile, "%s =>  UNIFORM BUFFER DESTROYED SUCCESSFULLY.\n", __func__);
    }

    if (uniformData.vkDeviceMemory)
    {
        vkFreeMemory(vkDevice, uniformData.vkDeviceMemory, NULL);
        uniformData.vkDeviceMemory = VK_NULL_HANDLE;
        fprintf(gpFile, "%s => UNIFORM DeviceMemory FREE SUCCESSFULLY.\n", __func__);
    }

    ShaderModuleHelper::DestroyShaderModule(vertexShaderModule);
    vertexShaderModule = VK_NULL_HANDLE;

    ShaderModuleHelper::DestroyShaderModule(fragmentShaderModule);
    fragmentShaderModule = VK_NULL_HANDLE;
}

void FadeInFadeOut::createPipeline()
{
    // VERTEX INPUT STATE
    VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
    memset((void *)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));

    // POSITION
    vkVertexInputBindingDescription_array[0].binding = 0;                // SIMILAR TO GL_ARRAY_BUFFER, CORESPONDING TO LOCATION 0
    vkVertexInputBindingDescription_array[0].stride = sizeof(float) * 3; // x,y,z
    vkVertexInputBindingDescription_array[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // COLOR
    vkVertexInputBindingDescription_array[1].binding = 1;                // SIMILAR TO GL_ARRAY_BUFFER, CORESPONDING TO LOCATION 1
    vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 4; // r,g,b,a
    vkVertexInputBindingDescription_array[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2]; // RELATED TO SHADER ATTRIBUTE
    memset((void *)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));

    // POSITION
    vkVertexInputAttributeDescription_array[0].binding = 0;
    vkVertexInputAttributeDescription_array[0].location = 0;                        // layout(location = 0) (shader line)
    vkVertexInputAttributeDescription_array[0].format = VK_FORMAT_R32G32B32_SFLOAT; // SIGNED FLOAT, 32 BIT
    vkVertexInputAttributeDescription_array[0].offset = 0;                          // USED FOR INTERLEAVED

    // COLOR
    vkVertexInputAttributeDescription_array[1].binding = 1;
    vkVertexInputAttributeDescription_array[1].location = 1;                           // layout(location = 0) (shader line)
    vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32A32_SFLOAT; // SIGNED FLOAT, 32 BIT
    vkVertexInputAttributeDescription_array[1].offset = 0;                             // USED FOR INTERLEAVED

    VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
    memset((void *)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

    vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vkPipelineVertexInputStateCreateInfo.pNext = NULL;
    vkPipelineVertexInputStateCreateInfo.flags = 0;
    vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = _ARRAYSIZE(vkVertexInputBindingDescription_array);
    vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vkVertexInputBindingDescription_array;
    vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
    vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vkVertexInputAttributeDescription_array;

    // SHADER STAGE
    VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2];
    memset((void *)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));

    // VERTEX SHADER
    vkPipelineShaderStageCreateInfo_array[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo_array[0].pNext = NULL;
    vkPipelineShaderStageCreateInfo_array[0].flags = 0;
    vkPipelineShaderStageCreateInfo_array[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineShaderStageCreateInfo_array[0].module = vertexShaderModule;
    vkPipelineShaderStageCreateInfo_array[0].pName = "main";
    vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    // FRAGMENT SHADER
    vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
    vkPipelineShaderStageCreateInfo_array[1].flags = 0;
    vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineShaderStageCreateInfo_array[1].module = fragmentShaderModule;
    vkPipelineShaderStageCreateInfo_array[1].pName = "main";
    vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    pipelineBuilder->mShaderStages.resize(2);
    pipelineBuilder->mShaderStages[0] = vkPipelineShaderStageCreateInfo_array[0];
    pipelineBuilder->mShaderStages[1] = vkPipelineShaderStageCreateInfo_array[1];
    pipelineBuilder->mVertexInputInfo = vkPipelineVertexInputStateCreateInfo;
    pipelineBuilder->mViewport = vkViewport;
    pipelineBuilder->mScissor = vkRect2D_Scissor;
    pipelineBuilder->mPipelineLayout = vkPipelineLayout;

    printf("before BuildPipeline \n");
    vkPipeline = pipelineBuilder->BuildPipeline(vkDevice, vkRenderPass, true);
    printf("after BuildPipeline \n");
}

VkResult FadeInFadeOut::createDescriptorSet()
{
    // variable declarations
    VkResult vkResult = VK_SUCCESS;

    // code

    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
    memset((void *)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

    vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    vkDescriptorSetAllocateInfo.pNext = NULL;
    vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
    vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
    vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

    vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkAllocateDescriptorSets() IS FAILED.\n", __func__);
        return (vkResult);
    }
    else
    {
        fprintf(gpFile, "%s => vkAllocateDescriptorSets() IS SUCCEEDED.\n", __func__);
    }

    // DESCRIBE WHETHER WE WANT BUFFER AS UNIFORM OR IMAGE AS UNIFORM
    VkDescriptorBufferInfo vkDescriptorBufferInfo;
    memset((void *)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

    vkDescriptorBufferInfo.buffer = uniformData.vkBuffer;
    vkDescriptorBufferInfo.offset = 0;
    vkDescriptorBufferInfo.range = sizeof(MyUniformData);

    // UPDATE THE ABOVE DESCRIPTOR SET DIRECLTY TO SHADER
    // THERE ARE TWO WE TO UPDATE WRITING DIRECTLY TO SHADER  oR COPYING ONE SHADFER TO ANOTHER SHADER
    // WE WILL PREFEER DIRECTLY WRITING TO SHADER
    VkWriteDescriptorSet vkWriteDescriptorSet;
    memset((void *)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

    vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vkWriteDescriptorSet.dstSet = vkDescriptorSet;
    vkWriteDescriptorSet.dstArrayElement = 0;
    vkWriteDescriptorSet.descriptorCount = 1;
    vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
    vkWriteDescriptorSet.pImageInfo = NULL;
    vkWriteDescriptorSet.pTexelBufferView = NULL;
    vkWriteDescriptorSet.dstBinding = 0;

    vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);
    fprintf(gpFile, "%s => vkUpdateDescriptorSets() IS SUCCEEDED.\n", __func__);

    return vkResult;
}

void FadeInFadeOut::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    // BIND WITH THE PIPELINE
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

    // BIND OUR DESCRIPTOR SET TO PIPELINE
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet, 0, NULL);

    quad->initialCommandBuffer(commandBuffer);
}

void FadeInFadeOut::update()
{
    // code

    if (alpha >= 1.0f)
    {
        isFadingIn = VK_TRUE;
    }
    else if (alpha <= 0.0f)
    {
        isFadingIn = VK_FALSE;
    }

    if (isFadingIn)
    {
        alpha = alpha - 0.0001f;
    }
    else
    {
        alpha = alpha + 0.0001f;
    }
}

void FadeInFadeOut::update(float _alpha)
{
    // code
    alpha = _alpha;
}

void FadeInFadeOut::display()
{
    updateUniformBuffer();
}

void FadeInFadeOut::onResize(const int width, const int height)
{
    if (vkPipeline)
    {
        vkDestroyPipeline(vkDevice, vkPipeline, NULL);
        vkPipeline = VK_NULL_HANDLE;
    }

    if (vkPipelineLayout)
    {
        vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
        vkPipelineLayout = VK_NULL_HANDLE;
    }

    createPipelineLayout();

    createPipeline();
}

VkResult FadeInFadeOut::updateUniformBuffer()
{
    // variable declarations

    VkResult vkResult = VK_SUCCESS;

    // code

    do
    {
        MyUniformData myUniformData;
        memset(&myUniformData, 0, sizeof(MyUniformData));

        // update matrices
        myUniformData.modelMatrix = glm::mat4(1.0f);
        myUniformData.viewMatrix = glm::mat4(1.0f);
        myUniformData.projectionMatrix = glm::mat4(1.0f);
        myUniformData.blendFactor = alpha;

        myUniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

        glm::mat4 perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
        perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f);

        myUniformData.projectionMatrix = perspectiveProjectionMatrix;

        // map uniform data
        void *data = NULL;
        vkResult = vkMapMemory(
            vkDevice,                   // [in] kontya device var map karaichi?
            uniformData.vkDeviceMemory, // [in] konti device memory? handle dya
            0,                          // [in] 0th offset pasun map kar
            sizeof(MyUniformData),      // [in] memory allocation size je
            0,                          // [in] must be 0 (reserved)
            &data                       // [in] host madhe memory bind kelela handle
        );
        if (vkResult != VK_SUCCESS)
        {
            // LOG_ERROR("vkMapMemory() failed.\n");
            break;
        }

        // copy uniform data into the buffer
        memcpy(data, &myUniformData, sizeof(MyUniformData));

        // unmap the uniform data
        vkUnmapMemory(vkDevice, uniformData.vkDeviceMemory);

    } while (0);

    return vkResult;
}

VkResult FadeInFadeOut::createUniformBuffer()
{
    // function declarations

    VkResult updateUniformBuffer(void);

    // variable declarations

    VkResult vkResult = VK_SUCCESS;

    // code

    do
    {
        memset(&uniformData, 0, sizeof(UniformData));

        VkBufferCreateInfo vkBufferCreateInfo;
        memset(&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.pNext = NULL;
        // valid flags are used in sparse (scattered) buffer
        vkBufferCreateInfo.flags = 0;
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        vkBufferCreateInfo.size = sizeof(struct MyUniformData);

        vkResult = vkCreateBuffer(
            vkDevice,             // [in] kontya device sathi buffer havay?
            &vkBufferCreateInfo,  // [in] memory kashya sathi vaparnar, etc tyachi mahiti
            NULL,                 // [in] custom memory allocator
            &uniformData.vkBuffer // [out] buffer jo banvun milnare
        );
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "[%s:%d] vkCreateBuffer() failed.\n", __FILE__, __LINE__);
            break;
        }

        VkMemoryRequirements vkMemoryRequirements;
        memset(&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

        vkGetBufferMemoryRequirements(
            vkDevice,             // [in] kontya device sathi ?
            uniformData.vkBuffer, // [in] kiti memory laagnare, jya buffer kade ahe to dya
            &vkMemoryRequirements // [out] memory requirements je bharun milnaret
        );

        VkMemoryAllocateInfo vkMemoryAllocateInfo;
        memset(&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

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

        vkResult = vkAllocateMemory(
            vkDevice,                   // [in] kontya device sathi memory haviye?
            &vkMemoryAllocateInfo,      // [in] kiti memory haviye and type kay? hya chi mahiti
            NULL,                       // [in] custom memory allocator
            &uniformData.vkDeviceMemory // [out] device memory handle je bharun milnare
        );
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "[%s:%d] vkAllocateMemory() failed.\n", __FILE__, __LINE__);
            break;
        }

        // bind device memory handle with Vulkan buffer handle
        vkResult = vkBindBufferMemory(
            vkDevice,                   // [in] kontya device var map karaicha?
            uniformData.vkBuffer,       // [in] konta buffer karaicha?
            uniformData.vkDeviceMemory, // [in] konti device memory karaichi map?
            0                           // [in] memory offset - kuthun bind karaichi ahe memory? start index
        );
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "[%s:%d] vkBindBufferMemory() failed.\n", __FILE__, __LINE__);
            break;
        }

        // update the uniform buffer
        vkResult = updateUniformBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "[%s:%d] updateUniformBuffer() failed.\n", __FILE__, __LINE__);
            break;
        }

    } while (0);

    return vkResult;
}

VkResult FadeInFadeOut::createDescriptorSetLayout()
{
    // variable declarations

    VkResult vkResult = VK_SUCCESS;

    // code

    do
    {
        VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo;
        memset(&vkDescriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

        vkDescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        vkDescriptorSetLayoutCreateInfo.pNext = NULL;
        vkDescriptorSetLayoutCreateInfo.flags = 0; // reserved

        // this is an array of VkDescriptorSetLayoutBinding structure and it's members are:

        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
        memset(&vkDescriptorSetLayoutBinding, 0, sizeof(VkDescriptorSetLayoutBinding));

        //! type of descriptors being provided e.g. uniforms, samplers, storage buffers, etc
        vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

        //!  this is the binding point similar to OpenGL's GL_ARRAY_BUFFER, GL_TEXTURE_2D. In OpenGL these were named binding points, but in Vulkan now programmer has been given the freedom to specify a number of his choice to be used for binding point.
        // this is the binding point in the shader i.e. layout(binding=0) uniform MVPMatrix line in the shader
        vkDescriptorSetLayoutBinding.binding = 0;

        //! number of descriptors provided
        vkDescriptorSetLayoutBinding.descriptorCount = 1;

        //! where to use this descriptor? in which shader / shaders?
        vkDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //! constant sampler (non-changing / permanent) samplers
        vkDescriptorSetLayoutBinding.pImmutableSamplers = NULL;

        vkDescriptorSetLayoutCreateInfo.pBindings = &vkDescriptorSetLayoutBinding;
        vkDescriptorSetLayoutCreateInfo.bindingCount = 1;

        vkResult = vkCreateDescriptorSetLayout(vkDevice,                         // [in] kontya device sathi create karaichay?
                                               &vkDescriptorSetLayoutCreateInfo, // [in] kontya descriptors ni create karaichay?
                                               NULL,                             // [in] custom memory allocator
                                               &vkDescriptorSetLayout            // [out] bharun milnara descriptor set layout
        );
        if (vkResult != VK_SUCCESS)
        {
            // LOG_ERROR("vkCreateDescriptorSetLayout() failed.\n");
            break;
        }

    } while (0);

    return vkResult;
}

VkResult FadeInFadeOut::createPipelineLayout()
{
    // variable declarations

    VkResult vkResult = VK_SUCCESS;

    // code

    do
    {
        VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo;
        memset(&vkPipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

        vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vkPipelineLayoutCreateInfo.pNext = NULL;
        vkPipelineLayoutCreateInfo.flags = 0;

        vkPipelineLayoutCreateInfo.setLayoutCount = 1;
        vkPipelineLayoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout;
        vkPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        vkPipelineLayoutCreateInfo.pPushConstantRanges = NULL;

        vkResult = vkCreatePipelineLayout(vkDevice,                    //[in] kontya device chi pipeline?
                                          &vkPipelineLayoutCreateInfo, //[in] pipeline layout la laagnari mahiti
                                          NULL,                        // [in] custom memory allocator
                                          &vkPipelineLayout            // [out] pipeline layout jo milnare
        );
        if (vkResult != VK_SUCCESS)
        {
            // LOG_ERROR("vkCreatePipelineLayout() failed.\n");
            break;
        }

    } while (0);

    return vkResult;
}