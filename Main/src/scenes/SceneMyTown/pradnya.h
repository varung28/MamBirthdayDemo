#pragma once

#include "../../helper/Common.h"
#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"

class ScenePradnya
{
public:
    struct VertexData
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkDeviceMemory;
    };

    struct VertexData vertexData_position;
    struct VertexData vertexData_color;

    VkDeviceSize colorVertexBuffer_AllocationSize;

    // (Ortho Change 1(a)) : Uniform-related Declarations
    struct MyUniformData
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

    struct UniformData
    {
        VkBuffer vkBuffer;
        VkDeviceMemory vkDeviceMemory;
    };

    struct UniformData uniformData;

    // "Shaders"-related Global Variables
    // (Shaders) Step 1 : Write and build the shaders
    // (Shaders) Step 2 : Globally declare 2 shader module object variables of type 'VkShaderModule' to hold Vulkan-compatible Vertex Shader Module and Fragment Shader Module
    VkShaderModule vkShaderModule_vertex_shader = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_fragment_shader = VK_NULL_HANDLE;

    // "Descriptor Set Layout"-related Global Variables
    // (Descriptor Set Layout) Step 1 : Globally declare Vulkan Object of type 'VkDescriptorSetLayout' and initialize it to VK_NULL_HANDLE
    VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    // "Pipeline Layout"-related Global Variables
    // (Pipeline Set Layout) Step 1 : Globally declare Vulkan Object of type 'VkPipelineLayout' and initialize it to VK_NULL_HANDLE
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    // (Ortho Change 1(b)) : Descriptor Pool
    VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;

    // (Ortho Change 1(c)) : Descriptor Set
    VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;

    // "Pipeline"-related Global Variables
    // (Pipeline) Step 1 : Declare following 3 variables globally
    VkViewport vkViewport;
    VkRect2D vkRect2D_scissor;
    VkPipeline vkPipeline = VK_NULL_HANDLE;

    VkResult initialize(void)
    {
        // // function prototypes
        // // VkResult createVertexBuffer(void);
        // VkResult createUniformBuffer(void);
        // // VkResult createVertexBuffer(void);        // (Vertex Buffer) Step 2(a) : Declaring prototype of createVertexBuffer() at this specific position.
        // VkResult createUniformBuffer(void); // Ortho Change 2(a)
        // // VkResult createShaders(void);             // (Shaders) Step 3 : Declaring prototype of createShaders() at this specific position.
        // VkResult createDescriptorSetLayout(void); // (Descriptor Set Layout) Step 2 : In initialize(), declare and call a UDF createDescriptorSetLayout() maintaining the convention of declaring and calling it AFTER createShaders() and BEFORE createRenderPass()
        // VkResult createPipelineLayout(void);      // (Pipeline Layout) Step 2 : In initialize(), declare and call a UDF createPipelineLayout() maintaining the convention of declaring and calling it AFTER createDescriptorSetLayout() and BEFORE createRenderPass()
        // VkResult createDescriptorPool(void);      // Ortho Change 2(b)
        // VkResult createDescriptorSet(void);       // Ortho Change 2(c)
        // //VkResult createPipeline(void);            // (Pipeline) Step 2(a) : Declaring prototype of createPipeline() at this specific position.
        // VkResult buildCommandBuffers(void);

        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // Create Vertex Buffer
        // (Vertex Buffer) Step 2(b) : Calling createVertexBuffer() at this specific position.
        vkResult = this->createVertexBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createVertexBuffer() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createVertexBuffer() Succeeded.\n", __func__);
        }

        // Ortho Change 3(a)
        fprintf(gpFile, "%s() : createUniformBuffer() PRADNYA.\n", __func__);
        vkResult = createUniformBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createUniformBuffer() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createUniformBuffer() Succeeded.\n", __func__);
        }

        // Create Shaders
        // (Shaders) Step 4 : Calling createShaders() at this specific position.
        vkResult = createShaders();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createShaders() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createShaders() Succeeded.\n", __func__);
        }

        // Create Descriptor Set Layout
        // (Descriptor Set Layout) Step 2 : In initialize(), declare and call a UDF createDescriptorSetLayout() maintaining the convention of declaring and calling it AFTER createShaders() and BEFORE createRenderPass()
        vkResult = createDescriptorSetLayout();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createDescriptorSetLayout() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createDescriptorSetLayout() Succeeded.\n", __func__);
        }

        // Create Pipeline Layout
        // (Pipeline Layout) Step 2 : In initialize(), declare and call a UDF createPipelineLayout() maintaining the convention of declaring and calling it AFTER createDescriptorSetLayout() and BEFORE createRenderPass()
        vkResult = createPipelineLayout();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createPipelineLayout() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createPipelineLayout() Succeeded.\n", __func__);
        }

        // Ortho Change 3(b)
        vkResult = createDescriptorPool();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createDescriptorPool() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createDescriptorPool() Succeeded.\n", __func__);
        }

        // Ortho Change 3(c)
        vkResult = createDescriptorSet();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createDescriptorSet() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createDescriptorSet() Succeeded.\n", __func__);
        }

        // Create Pipeline
        // (Pipeline) Step 2(b) : Calling createPipeline() at this specific position.
        vkResult = createPipeline();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createPipeline() Failed.\n", __func__);
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : createPipeline() Succeeded.\n", __func__);
        }

        // // Build Command Buffers
        // vkResult = buildCommandBuffers();
        // if (vkResult != VK_SUCCESS)
        // {
        //     fprintf(gpFile, "%s() : buildCommandBuffers() Failed.\n", __func__);
        //     return (vkResult);
        // }
        // else
        // {
        //     fprintf(gpFile, "%s() : buildCommandBuffers() Succeeded.\n", __func__);
        // }

        // fprintf(gpFile, "%s() Completed Successfully.\n", __func__);

        // // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        // fprintf(gpFile, "===================================\n");
        // fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        // fprintf(gpFile, "===================================\n");
        // // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    VkResult resize(int width, int height)
    {
        // function prototypes (Those functions called in initialize() which are dependent upon 'swapchainImageCount' in one way or the other)

        // varaible declarations
        VkResult vkResult = VK_SUCCESS;

        // code

        // Wait for the device to complete the in-hand tasks
        if (vkDevice)
        {
            vkDeviceWaitIdle(vkDevice);
            fprintf(gpFile, "%s() : vkDeviceWaitIdle() Is Done!\n", __func__);
        }

        // (Pipeline) Step 4 : Destroy the Pipeline
        if (vkPipeline)
        {
            vkDestroyPipeline(vkDevice, vkPipeline, NULL);
            vkPipeline = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyPipeline() Succeeded.\n", __func__);
        }

        // (Pipeline Layout) Step 5 : In uninitialize(), call vkDestroyPipelineLayout() to destroy 'vkPipelineLayout' Vulkan Object.
        if (vkPipelineLayout)
        {
            vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
            vkPipelineLayout = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyPipelineLayout() Succeeded.\n", __func__);
        }

        // ************ RE-CREATE FOR RESIZING ************

        // Create Pipeline Layout
        // (Pipeline Layout) Step 2 : In initialize(), declare and call a UDF createPipelineLayout() maintaining the convention of declaring and calling it AFTER createDescriptorSetLayout() and BEFORE createRenderPass()
        vkResult = createPipelineLayout();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createPipelineLayout() Failed.\n", __func__);
            return (vkResult);
        }

        // Create Pipeline
        // (Pipeline) Step 2(b) : Calling createPipeline() at this specific position.
        vkResult = createPipeline();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : createPipeline() Failed.\n", __func__);
            return (vkResult);
        }

        return (vkResult);
    }

    VkResult update(void)
    {
        VkResult vkResult = VK_SUCCESS;

        // (Ortho Change 4(b)) : Update Uniform Buffer
        vkResult = updateUniformBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : updateUniformBuffer() Failed.\n", __func__);
            return (vkResult);
        }

        vkResult = updateColorVertexBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : updateColorVertexBuffer() Failed.\n", __func__);
            return (vkResult);
        }
        return (vkResult);
    }

    void uninitialize(void)
    {
        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        vkDeviceWaitIdle(vkDevice);

        // (Pipeline) Step 4 : Destroy the Pipeline
        if (vkPipeline)
        {
            vkDestroyPipeline(vkDevice, vkPipeline, NULL);
            vkPipeline = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyPipeline() Succeeded.\n", __func__);
        }

        // (Ortho Change 5(a)) : Destroy Descriptor Pool (When Descriptor Pool is destroyed, the descriptor sets created by that pool are autmatically destroyed)
        if (vkDescriptorPool)
        {
            vkDestroyDescriptorPool(vkDevice, vkDescriptorPool, NULL);
            vkDescriptorPool = VK_NULL_HANDLE;
            vkDescriptorSet = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyDescriptorPool() Succeeded In Destroying Descriptor Pool and Descriptor Set.\n", __func__);
        }

        // (Pipeline Layout) Step 5 : In uninitialize(), call vkDestroyPipelineLayout() to destroy 'vkPipelineLayout' Vulkan Object.
        if (vkPipelineLayout)
        {
            vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
            vkPipelineLayout = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyPipelineLayout() Succeeded.\n", __func__);
        }

        // (Descriptor Set Layout) Step 5 : In uninitialize(), call vkDestroyDescriptorSetLayout() to destroy 'vkDescriptorSetLayout' Vulkan Object.
        if (vkDescriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, NULL);
            vkDescriptorSetLayout = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyDescriptorSetLayout() Succeeded.\n", __func__);
        }

        // ********* SHADERS UNINITIALIZATIONS (Start) *********
        // (Shaders) Step 11(a) : Free the globally declared Fragment Shader Module
        if (vkShaderModule_fragment_shader)
        {
            vkDestroyShaderModule(vkDevice, vkShaderModule_fragment_shader, NULL);
            vkShaderModule_fragment_shader = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyShaderModule() Succeeded For Fragment Shader.\n", __func__);
        }

        // (Shaders) Step 11(a) : Free the globally declared Vertex Shader Module
        if (vkShaderModule_vertex_shader)
        {
            vkDestroyShaderModule(vkDevice, vkShaderModule_vertex_shader, NULL);
            vkShaderModule_vertex_shader = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyShaderModule() Succeeded For Vertex Shader.\n", __func__);
        }
        // ********* SHADERS UNINITIALIZATIONS (End) *********

        // ********* UNIFORM BUFFER UNINITIALIZATIONS (Start) *********
        // (Ortho Change 5(b)) : Free .vkDeviceMemory member of global variable 'uniformData'
        if (uniformData.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, uniformData.vkDeviceMemory, NULL);
            uniformData.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkFreeMemory() Succeeded For uniformData.vkDeviceMemory.\n", __func__);
        }

        // (Ortho Change 5(c)) : Free .vkBuffer member of global variable 'uniformData'
        if (uniformData.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, uniformData.vkBuffer, NULL);
            uniformData.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyBuffer() Succeeded For uniformData.vkBuffer.\n", __func__);
        }
        // ********* UNIFORM BUFFER UNINITIALIZATIONS (End) *********

        // ********* VERTEX BUFFERS UNINITIALIZATIONS (Start) *********

        // Uninitializing Vertex Color Buffer
        if (vertexData_color.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, vertexData_color.vkDeviceMemory, NULL);
            vertexData_color.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkFreeMemory() Succeeded For vertexData_color.vkDeviceMemory.\n", __func__);
        }

        if (vertexData_color.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, vertexData_color.vkBuffer, NULL);
            vertexData_color.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyBuffer() Succeeded For vertexData_color.vkBuffer.\n", __func__);
        }

        // Uninitializing Vertex Position Buffer
        if (vertexData_position.vkDeviceMemory)
        {
            vkFreeMemory(vkDevice, vertexData_position.vkDeviceMemory, NULL);
            vertexData_position.vkDeviceMemory = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkFreeMemory() Succeeded For vertexData_position.vkDeviceMemory.\n", __func__);
        }

        if (vertexData_position.vkBuffer)
        {
            vkDestroyBuffer(vkDevice, vertexData_position.vkBuffer, NULL);
            vertexData_position.vkBuffer = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyBuffer() Succeeded For vertexData_position.vkBuffer.\n", __func__);
        }
    }

    // (Vertex Buffer) Step 2(c) : Defining createVertexBuffer() at this specific position
    VkResult createVertexBuffer(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // (Vertex Buffer) Step 1 : In global variable declarations

        // (Vertex Buffer) Step 2 : In initialize()

        // (Vertex Buffer) Step 3 : Declare Triangle's Positions Array
        float rectangle_position[] =
            {
                // First Triangle
                1.0f, 1.0f, 0.0f,   // right-top
                -1.0f, 1.0f, 0.0f,  // left-top
                -1.0f, -1.0f, 0.0f, // left-bottom

                // Second Triangle
                -1.0f, -1.0f, 0.0f, // left-bottom
                1.0f, -1.0f, 0.0,   // right-bottom
                1.0f, 1.0f, 0.0f    // right-top
            };

        // Multicolored Triangle Change 3
        // Declare Triangle's Colors Array
        float rectangle_color[] =
            {
                // First Triangle
                1.0f,
                0.0f,
                0.0f,
                1.0f,
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f,

                // Second Triangle
                0.0f,
                0.0f,
                1.0f,
                0.0f,
                0.0f,
                1.0f,
                1.0f,
                0.0f,
                0.0f,
            };

        // ************ VERTEX POSITION BUFFER ************

        // (Vertex Buffer) Step 4 : memset() our global 'vertexData_position' struct variable
        memset((void *)&vertexData_position, 0, sizeof(struct VertexData));

        // (Vertex Buffer) Step 5 : Declare and memset() 'VkBufferCreateInfo' structure. It has 8 members - we will use 5 of them - and out of them 2 are very important (.usage and .size)
        VkBufferCreateInfo vkBufferCreateInfo;
        memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.pNext = NULL;
        vkBufferCreateInfo.flags = 0;                         // Valid flags are used for scattered / sparse buffers
        vkBufferCreateInfo.size = sizeof(rectangle_position); // memory specified in bytes - but will be allocated in regions
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        // (Vertex Buffer) Step 6 : Call vkCreateBuffer() in the .vkBuffer member of our global variable 'vertexData_position'
        vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position.vkBuffer);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Failed For Vertex Position Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Succeeded For Vertex Position Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 7 : Declare and memset() 'VkMemoryRequirements' struct and then call vkGetBufferMemoryRequirements() to get the memory requirements
        VkMemoryRequirements vkMemoryRequirements;
        memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

        vkGetBufferMemoryRequirements(vkDevice, vertexData_position.vkBuffer, &vkMemoryRequirements);

        // (Vertex Buffer) Step 8 : To actually allocate the required memory - we need to call vkAllocateMemory(). But before that, we need to declare and memset() 'VkMemoryAllocateInfo' structure.
        // Important Members of 'VkMemoryAllocateInfo' structure are : 1) .memoryTypeIndex and 2) .allocationSize
        // For .allocationSize member, use the size obtained from memory requirements in Step 7 above.
        VkMemoryAllocateInfo vkMemoryAllocateInfo;
        memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

        vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vkMemoryAllocateInfo.pNext = NULL;
        // No .flags member
        vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size; // size allocated in regions

        // For .memoryTypeIndex member :
        vkMemoryAllocateInfo.memoryTypeIndex = 0; // initial value before entering loop

        // Step 8(a) : Start a loop with count as 'vkPhysicalDeviceMemoryProperties.memoryTypeCount' (05-PhysicalDevice, Step 1)
        for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            // Step 8(b) : Check vkMemoryRequirements.memoryTypeBits contains 1 or not. If it contains 1 ...
            if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
            {
                // Step 8(c) : Check whether 'vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags' member contains the enum 'VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT'
                if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) // If yes ...
                {
                    // Step 8(d) : Then the ith index is out '.memoryTypeIndex'. If found, break out of the loop ...
                    vkMemoryAllocateInfo.memoryTypeIndex = i;
                    break;
                }
            }

            // Step 8(e) : If memoryTypeIndex is not found, continue the loop by right-shifting vkMemoryRequirements.memoryTypeBits by 1 bit, over each iteration
            vkMemoryRequirements.memoryTypeBits >>= 1;
        }

        // (Vertex Buffer) Step 9 : Now call vkAllocateMemory() and get the required memory object's handle into the .vkDeviceMemory member of our global variable 'vertexData_position'
        vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position.vkDeviceMemory);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Failed For Vertex Position Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Succeeded For Vertex Position Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 10 : Now we have our required device memory handle and vkBuffer handle.
        // Bind this device memory handle to the Vulkan Buffer handle using vkBindBufferMemory()
        vkResult = vkBindBufferMemory(vkDevice, vertexData_position.vkBuffer, vertexData_position.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Failed For Vertex Position Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Succeeded For Vertex Position Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 11 : Declare a local void * buffer, say 'data', and call vkMapMemory() to map our device memory object handle to this void * data
        // This will allow us to do MMIO.
        // Means - when we write on to void *data, it will automatically be written / copied to the device memory represented by device memory object handle.
        void *data = NULL;
        vkResult = vkMapMemory(vkDevice, vertexData_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkMapMemory() Failed For Vertex Position Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkMapMemory() Succeeded For Vertex Position Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 12 : Now to do actual MMIO, call memcpy()
        memcpy(data, rectangle_position, sizeof(rectangle_position));

        // (Vertex Buffer) Step 13 : To complete this MMIO, finally call vkUnmapMmeory()
        vkUnmapMemory(vkDevice, vertexData_position.vkDeviceMemory);

        // (Vertex Buffer) Step 14 : In uninitialize() ...

        // ************ VERTEX COLOR BUFFER ************ (Multicolored Triangle Change 4)

        memset((void *)&vertexData_color, 0, sizeof(struct VertexData));

        // (Vertex Buffer) Step 5 : Declare and memset() 'VkBufferCreateInfo' structure. It has 8 members - we will use 5 of them - and out of them 2 are very important (.usage and .size)
        memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.pNext = NULL;
        vkBufferCreateInfo.flags = 0;                      // Valid flags are used for scattered / sparse buffers
        vkBufferCreateInfo.size = sizeof(rectangle_color); // memory specified in bytes - but will be allocated in regions
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        // (Vertex Buffer) Step 6 : Call vkCreateBuffer() in the .vkBuffer member of our global variable 'vertexData_position'
        vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color.vkBuffer);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Succeeded For Vertex Color Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 7 : Declare and memset() 'VkMemoryRequirements' struct and then call vkGetBufferMemoryRequirements() to get the memory requirements
        memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

        vkGetBufferMemoryRequirements(vkDevice, vertexData_color.vkBuffer, &vkMemoryRequirements);

        // (Vertex Buffer) Step 8 : To actually allocate the required memory - we need to call vkAllocateMemory(). But before that, we need to declare and memset() 'VkMemoryAllocateInfo' structure.
        // Important Members of 'VkMemoryAllocateInfo' structure are : 1) .memoryTypeIndex and 2) .allocationSize
        // For .allocationSize member, use the size obtained from memory requirements in Step 7 above.
        memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

        vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vkMemoryAllocateInfo.pNext = NULL;
        // No .flags member
        vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size; // size allocated in regions
        colorVertexBuffer_AllocationSize = vkMemoryAllocateInfo.allocationSize;

        // For .memoryTypeIndex member :
        vkMemoryAllocateInfo.memoryTypeIndex = 0; // initial value before entering loop

        // Step 8(a) : Start a loop with count as 'vkPhysicalDeviceMemoryProperties.memoryTypeCount' (05-PhysicalDevice, Step 1)
        for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            // Step 8(b) : Check vkMemoryRequirements.memoryTypeBits contains 1 or not. If it contains 1 ...
            if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
            {
                // Step 8(c) : Check whether 'vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags' member contains the enum 'VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT'
                if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) // If yes ...
                {
                    // Step 8(d) : Then the ith index is out '.memoryTypeIndex'. If found, break out of the loop ...
                    vkMemoryAllocateInfo.memoryTypeIndex = i;
                    break;
                }
            }

            // Step 8(e) : If memoryTypeIndex is not found, continue the loop by right-shifting vkMemoryRequirements.memoryTypeBits by 1 bit, over each iteration
            vkMemoryRequirements.memoryTypeBits >>= 1;
        }

        // (Vertex Buffer) Step 9 : Now call vkAllocateMemory() and get the required memory object's handle into the .vkDeviceMemory member of our global variable 'vertexData_position'
        vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color.vkDeviceMemory);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Succeeded For Vertex Color Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 10 : Now we have our required device memory handle and vkBuffer handle.
        // Bind this device memory handle to the Vulkan Buffer handle using vkBindBufferMemory()
        vkResult = vkBindBufferMemory(vkDevice, vertexData_color.vkBuffer, vertexData_color.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Succeeded For Vertex Color Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 11 : Declare a local void * buffer, say 'data', and call vkMapMemory() to map our device memory object handle to this void * data
        // This will allow us to do MMIO.
        // Means - when we write on to void *data, it will automatically be written / copied to the device memory represented by device memory object handle.
        vkResult = vkMapMemory(vkDevice, vertexData_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkMapMemory() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkMapMemory() Succeeded For Vertex Color Buffer.\n", __func__);
        }

        // (Vertex Buffer) Step 12 : Now to do actual MMIO, call memcpy()
        memcpy(data, rectangle_color, sizeof(rectangle_color));

        // (Vertex Buffer) Step 13 : To complete this MMIO, finally call vkUnmapMmeory()
        vkUnmapMemory(vkDevice, vertexData_color.vkDeviceMemory);

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    // (Ortho Change 7(a)) : Defining createUniformBuffer()
    VkResult createUniformBuffer(void)
    {
        // function prototypes

        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // memset() our global 'uniformData' struct variable
        memset((void *)&uniformData, 0, sizeof(struct UniformData));

        // Declare and memset() 'VkBufferCreateInfo' structure. It has 8 members - we will use 5 of them - and out of them 2 are very important (.usage and .size)
        VkBufferCreateInfo vkBufferCreateInfo;
        memset((void *)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.pNext = NULL;
        vkBufferCreateInfo.flags = 0;                           // Valid flags are used for scattered / sparse buffers
        vkBufferCreateInfo.size = sizeof(struct MyUniformData); // memory specified in bytes - but will be allocated in regions
        vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

        // Call vkCreateBuffer() in the .vkBuffer member of our global variable 'uniformData'
        vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData.vkBuffer);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Failed For Uniform Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateBuffer() Succeeded For Uniform Buffer.\n", __func__);
        }

        // Declare and memset() 'VkMemoryRequirements' struct and then call vkGetBufferMemoryRequirements() to get the memory requirements
        VkMemoryRequirements vkMemoryRequirements;
        memset((void *)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

        vkGetBufferMemoryRequirements(vkDevice, uniformData.vkBuffer, &vkMemoryRequirements);

        // To actually allocate the required memory - we need to call vkAllocateMemory(). But before that, we need to declare and memset() 'VkMemoryAllocateInfo' structure.
        // Important Members of 'VkMemoryAllocateInfo' structure are : 1) .memoryTypeIndex and 2) .allocationSize
        // For .allocationSize member, use the size obtained from memory requirements in Step 7 above.
        VkMemoryAllocateInfo vkMemoryAllocateInfo;
        memset((void *)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

        vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vkMemoryAllocateInfo.pNext = NULL;
        // No .flags member
        vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size; // size allocated in regions

        // For .memoryTypeIndex member :
        vkMemoryAllocateInfo.memoryTypeIndex = 0; // initial value before entering loop

        // Start a loop with count as 'vkPhysicalDeviceMemoryProperties.memoryTypeCount' (05-PhysicalDevice, Step 1)
        for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            // Check vkMemoryRequirements.memoryTypeBits contains 1 or not. If it contains 1 ...
            if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
            {
                // Check whether 'vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags' member contains the enum 'VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT'
                if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) // If yes ...
                {
                    // Then the ith index is out '.memoryTypeIndex'. If found, break out of the loop ...
                    vkMemoryAllocateInfo.memoryTypeIndex = i;
                    break;
                }
            }

            // If memoryTypeIndex is not found, continue the loop by right-shifting vkMemoryRequirements.memoryTypeBits by 1 bit, over each iteration
            vkMemoryRequirements.memoryTypeBits >>= 1;
        }

        // Now call vkAllocateMemory() and get the required memory object's handle into the .vkDeviceMemory member of our global variable 'vertexData_position'
        vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData.vkDeviceMemory);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Failed For Uniform Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkAllocateMemory() Succeeded For Uniform Buffer.\n", __func__);
        }

        // Now we have our required device memory handle and vkBuffer handle.
        // Bind this device memory handle to the Vulkan Buffer handle using vkBindBufferMemory()
        vkResult = vkBindBufferMemory(vkDevice, uniformData.vkBuffer, uniformData.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Failed For Uniform Buffer.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Succeeded For Uniform Buffer.\n", __func__);
        }

        // Call updateUniformBuffer()
        vkResult = updateUniformBuffer();
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : updateUniformBuffer() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : updateUniformBuffer() Succeeded.\n", __func__);
        }

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    // (Ortho Change 7(b)) : Defining updateUniformBuffer()
    VkResult updateUniformBuffer(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        struct MyUniformData myUniformData;
        memset((void *)&myUniformData, 0, sizeof(struct MyUniformData));

        // Update Matrices
        myUniformData.modelMatrix = glm::mat4(1.0f);
        myUniformData.modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(3.0f, 3.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)); // (Perspective Change 2) : Translation

        myUniformData.viewMatrix = glm::mat4(1.0f);

        glm::mat4 perspectiveProjectionMatrix = glm::mat4(1.0f);

        // We used to do the following code in resize() in OpenGL
        // So why doing it here in Vulkan ?
        // Vulkan is multi-threaded and so may not ensure that display() will be called after resize()
        // That is why we are doing this code in updateUniformBuffer() which we have called in resize()
        perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f); // (Perspective Change 3) : Setting Perspective Projection Matrix

        // Corrected Y - Perspective Matrix Way - Change 2 => changing the 2n/(t - b) element of Perspective Projection Matrix
        perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f);

        myUniformData.projectionMatrix = perspectiveProjectionMatrix;

        // Map Uniform Buffer
        void *data = NULL;
        vkResult = vkMapMemory(vkDevice, uniformData.vkDeviceMemory, 0, sizeof(struct MyUniformData), 0, &data);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkMapMemory() Failed For Uniform Buffer.\n", __func__);
            return (vkResult);
        }

        // Copy the data
        memcpy(data, &myUniformData, sizeof(struct MyUniformData));

        // Unmap the Uniform Buffer
        vkUnmapMemory(vkDevice, uniformData.vkDeviceMemory);

        return (vkResult);
    }

    VkResult updateColorVertexBuffer(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code

        struct MyUniformData myUniformData;
        memset((void *)&myUniformData, 0, sizeof(struct MyUniformData));

        // Update Matrices
        const unsigned int numRows = 6;
        const unsigned int numColumns = 3;

        const float lighterGreen = 115.0f / 255.0f;
        const float darkerGreen = 30.0f / 255.0f;

        static float rectangle_colors_2D_array[numRows][numColumns] = {{0.0f, lighterGreen, 0.0f},
                                                                       {0.0f, lighterGreen, 0.0f},
                                                                       {0.0f, lighterGreen, 0.0f},

                                                                       {0.0f, lighterGreen, 0.0f},
                                                                       {0.0f, lighterGreen, 0.0f},
                                                                       {0.0f, lighterGreen, 0.0f}};

        const float delta = 0.0001f;

        // ****** RED TO BLUE ******
        if (rectangle_colors_2D_array[0][1] > darkerGreen)
            rectangle_colors_2D_array[0][1] = rectangle_colors_2D_array[0][1] - delta;

        if (rectangle_colors_2D_array[1][1] > darkerGreen)
            rectangle_colors_2D_array[1][1] = rectangle_colors_2D_array[1][1] - delta;

        if (rectangle_colors_2D_array[5][1] > darkerGreen)
            rectangle_colors_2D_array[5][1] = rectangle_colors_2D_array[5][1] - delta;

        float rectangle_color[numRows * numColumns];

        int i, j;
        for (i = 0; i < numRows; i++)
        {
            for (j = 0; j < numColumns; j++)
            {
                rectangle_color[(i * numColumns) + j] = rectangle_colors_2D_array[i][j];
            }
        }

        // (Vertex Buffer) Step 10 : Now we have our required device memory handle and vkBuffer handle.
        // Bind this device memory handle to the Vulkan Buffer handle using vkBindBufferMemory()
        vkResult = vkBindBufferMemory(vkDevice, vertexData_color.vkBuffer, vertexData_color.vkDeviceMemory, 0);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkBindBufferMemory() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        // (Vertex Buffer) Step 11 : Declare a local void * buffer, say 'data', and call vkMapMemory() to map our device memory object handle to this void * data
        // This will allow us to do MMIO.
        // Means - when we write on to void *data, it will automatically be written / copied to the device memory represented by device memory object handle.
        void *data = NULL;
        vkResult = vkMapMemory(vkDevice, vertexData_color.vkDeviceMemory, 0, colorVertexBuffer_AllocationSize, 0, &data);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkMapMemory() Failed For Vertex Color Buffer.\n", __func__);
            return (vkResult);
        }

        // (Vertex Buffer) Step 12 : Now to do actual MMIO, call memcpy()
        memcpy(data, rectangle_color, sizeof(rectangle_color));

        // (Vertex Buffer) Step 13 : To complete this MMIO, finally call vkUnmapMmeory()
        vkUnmapMemory(vkDevice, vertexData_color.vkDeviceMemory);

        return (vkResult);
    }

    // (Shaders) Step 5 : Defining createShaders() at this specific position.
    VkResult createShaders(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // (Shaders) Step 1 : In .\Shaders directory (Writing and building of shader files)

        // (Shaders) Step 2 : In global variable declarations.

        // (Shaders) Step 3 : In initialize() - function prototype

        // (Shaders) Step 4 : In initialize() - function call

        // (Shaders) Step 5 : THIS function definition (see at the top of this function)

        // ********* VERTEX SHADER *********
        // (Shaders) Step 6 : Reading Shader File into a memory buffer
        const char *szShaderFileName = ".\\bin\\pradnya_shader.vert.spv";
        FILE *fp = NULL;
        size_t shaderFileSize = 0;

        // Step 6(a) : Open the Shader File.
        fp = fopen(szShaderFileName, "rb"); // read binary
        if (fp == NULL)
        {
            fprintf(gpFile, "%s() : Failed To Open %s Vertex Shader File.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : Successfully Opened %s Vertex Shader File.\n", __func__, szShaderFileName);
        }

        // Step 6(b) : Set the File Pointer at the End of the File.
        fseek(fp, 0L, SEEK_END);

        // Step 6(c) : Find the Byte Size of the Shader File Data.
        shaderFileSize = ftell(fp);
        if (shaderFileSize == 0)
        {
            fprintf(gpFile, "%s() :%s Vertex Shader File Contains NO data! File Size is 0 Bytes!\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }

        // Step 6(d) : Reset the File Pointer to the beginning of the File.
        fseek(fp, 0L, SEEK_SET);

        // Step 6(e) : Allocate a char buffer of "file size" and read the shader file data into it.
        char *shaderData = (char *)malloc(sizeof(char) * shaderFileSize);
        if (shaderData == NULL)
        {
            fprintf(gpFile, "%s() : Failed To Allocate Memory Buffer For %s Vertex Shader File Data.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }

        size_t retVal = fread(shaderData, shaderFileSize, 1, fp);
        if (retVal != 1)
        {
            fprintf(gpFile, "%s() : Failed To Read %s Vertex Shader File Data.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : Successfully Read %s Vertex Shader File Data.\n", __func__, szShaderFileName);
        }

        // Step 6(f) : Finally, close the file.
        if (fp)
        {
            fclose(fp);
            fp = NULL;
        }

        // (Shaders) Step 7 : Declare and memset() 'VkShaderModuleCreateInfo' and specify the above file size and buffer while initializing it.
        VkShaderModuleCreateInfo vkShaderModuleCreateInfo;
        memset((void *)&vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

        vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vkShaderModuleCreateInfo.pNext = NULL;
        vkShaderModuleCreateInfo.flags = 0; // reserved for future use, hence must be 0
        vkShaderModuleCreateInfo.codeSize = shaderFileSize;
        vkShaderModuleCreateInfo.pCode = (uint32_t *)shaderData;

        // (Shaders) Step 8 : Call vkCreateShaderModule(), pass the above structure's pointer to it as a parameter and obtain the required Shader Module Object in the respective global variable declared in Step 2.
        vkResult = vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_vertex_shader);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateShaderModule() Failed For %s Vertex Shader.\n", __func__, szShaderFileName);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateShaderModule() Succeeded For %s Vertex Shader.\n", __func__, szShaderFileName);
        }

        // (Shaders) Step 9 : Free the Shader Code Buffer which we allocated in Step 6(e).
        if (shaderData)
        {
            free(shaderData);
            shaderData = NULL;
        }

        fprintf(gpFile, "%s() : Vertex Shader Module (from file %s) Is Successfully Created!\n", __func__, szShaderFileName);

        // (Shaders) Step 10 : Repeat Steps 6 to 9 for all shaders
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // ********* FRAGMENT SHADER *********
        // (Shaders) Step 6 : Reading Shader File into a memory buffer
        szShaderFileName = ".\\bin\\pradnya_shader.frag.spv";
        fp = NULL;
        shaderFileSize = 0;

        // Step 6(a) : Open the Shader File.
        fp = fopen(szShaderFileName, "rb"); // read binary
        if (fp == NULL)
        {
            fprintf(gpFile, "%s() : Failed To Open %s Fragment Shader File.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : Successfully Opened %s Fragment Shader File.\n", __func__, szShaderFileName);
        }

        // Step 6(b) : Set the File Pointer at the End of the File.
        fseek(fp, 0L, SEEK_END);

        // Step 6(c) : Find the Byte Size of the Shader File Data.
        shaderFileSize = ftell(fp);
        if (shaderFileSize == 0)
        {
            fprintf(gpFile, "%s() :%s Fragment Shader File Contains NO data! File Size is 0 Bytes!\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }

        // Step 6(d) : Reset the File Pointer to the beginning of the File.
        fseek(fp, 0L, SEEK_SET);

        // Step 6(e) : Allocate a char buffer of "file size" and read the shader file data into it.
        shaderData = (char *)malloc(sizeof(char) * shaderFileSize);
        if (shaderData == NULL)
        {
            fprintf(gpFile, "%s() : Failed To Allocate Memory Buffer For %s Fragment Shader File Data.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }

        retVal = fread(shaderData, shaderFileSize, 1, fp);
        if (retVal != 1)
        {
            fprintf(gpFile, "%s() : Failed To Read %s Fragment Shader File Data.\n", __func__, szShaderFileName);
            vkResult = VK_ERROR_INITIALIZATION_FAILED;
            return (vkResult);
        }
        else
        {
            fprintf(gpFile, "%s() : Successfully Read %s Fragment Shader File Data.\n", __func__, szShaderFileName);
        }

        // Step 6(f) : Finally, close the file.
        if (fp)
        {
            fclose(fp);
            fp = NULL;
        }

        // (Shaders) Step 7 : Declare and memset() 'VkShaderModuleCreateInfo' and specify the above file size and buffer while initializing it.
        memset((void *)&vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

        vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vkShaderModuleCreateInfo.pNext = NULL;
        vkShaderModuleCreateInfo.flags = 0; // reserved for future use, hence must be 0
        vkShaderModuleCreateInfo.codeSize = shaderFileSize;
        vkShaderModuleCreateInfo.pCode = (uint32_t *)shaderData;

        // (Shaders) Step 8 : Call vkCreateShaderModule(), pass the above structure's pointer to it as a parameter and obtain the required Shader Module Object in the respective global variable declared in Step 2.
        vkResult = vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_fragment_shader);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateShaderModule() Failed For %s Fragment Shader.\n", __func__, szShaderFileName);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateShaderModule() Succeeded For %s Fragment Shader.\n", __func__, szShaderFileName);
        }

        // (Shaders) Step 9 : Free the Shader Code Buffer which we allocated in Step 6(e).
        if (shaderData)
        {
            free(shaderData);
            shaderData = NULL;
        }

        fprintf(gpFile, "%s() : Fragment Shader Module (from file %s) Is Successfully Created!\n", __func__, szShaderFileName);

        // (Shaders) Step 11 : In uninitialize() ...

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    VkResult createDescriptorSetLayout(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // (Ortho Change 8(a))
        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
        memset((void *)&vkDescriptorSetLayoutBinding, 0, sizeof(VkDescriptorSetLayoutBinding));

        vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkDescriptorSetLayoutBinding.binding = 0; // 0th binding point (given in vertex shader)
        vkDescriptorSetLayoutBinding.descriptorCount = 1;
        vkDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        vkDescriptorSetLayoutBinding.pImmutableSamplers = NULL;

        // (Descriptor Set Layout) Step 1 : Done globally ...

        // (Descriptor Set Layout) Step 2 : Done in initialize() ...

        // (Descriptor Set Layout) Step 3 : While defining UDF createDescriptorSetLayout() - declare, memset() and initialize struct 'VkDescriptorSetLayoutCreateInfo' - particularly, its two members : 1) bindingCount and 2) pBindings
        VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo;
        memset((void *)&vkDescriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

        vkDescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        vkDescriptorSetLayoutCreateInfo.pNext = NULL;
        vkDescriptorSetLayoutCreateInfo.flags = 0;                                 // reserved, hence, must be 0
        vkDescriptorSetLayoutCreateInfo.bindingCount = 1;                          // (Ortho Change 8(b))
        vkDescriptorSetLayoutCreateInfo.pBindings = &vkDescriptorSetLayoutBinding; // (Ortho Change 8(c))

        // 'pBindings' is a pointer of struct 'VkDescriptorSetLayoutBinding' - 5 members :
        // 1) uint32_t binding (like binding targets of OpenGL)
        // 2) VkDescriptorSetType descriptorType
        // 3) uint32_t descriptorCount
        // 4) VkShaderStageFlags stageFlags
        // 5) const VkSampler *pImmutableSamplers (usually not used)

        // (Descriptor Set Layout) Step 4 : Then call vkCreateDescriptorSetLayout() with address of above initialized structure as parameter and get the required global Vulkan Object 'vkDescriptorSetLayout' in its last parameter.
        vkResult = vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, NULL, &vkDescriptorSetLayout);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateDescriptorSetLayout() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateDescriptorSetLayout() Succeeded.\n", __func__);
        }

        // (Descriptor Set Layout) Step 5 : In uninitialize() ...

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    VkResult createPipelineLayout(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // (Pipeline Layout) Step 1 : Done globally ...

        // (Pipeline Layout) Step 2 : Done in initialize() ...

        // (Pipeline Layout) Step 3 : While defining UDF createPipelineLayout() - declare, memset() and initialize struct 'VkPipelineLayoutCreateInfo' - particularly, its 4 members : 1) setLayoutCount | 2) pSetLayouts | 3) pushConstantRangeCount | 4) pPushConstantRanges
        VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo;
        memset((void *)&vkPipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

        vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vkPipelineLayoutCreateInfo.pNext = NULL;
        vkPipelineLayoutCreateInfo.flags = 0; // reserved, hence, must be 0
        vkPipelineLayoutCreateInfo.setLayoutCount = 1;
        vkPipelineLayoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout;
        vkPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        vkPipelineLayoutCreateInfo.pPushConstantRanges = NULL;

        // (Pipeline Layout) Step 4 : Then call vkCreatePipelineLayout() with address of above initialized structure as parameter and get the required global Vulkan Object 'vkPipelineLayout' in its last parameter.
        vkResult = vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, NULL, &vkPipelineLayout);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreatePipelineLayout() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreatePipelineLayout() Succeeded.\n", __func__);
        }

        // (Pipeline Layout) Step 5 : In uninitialize() ...

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    // (Ortho Change 7(c)) : Defining createDescriptorPool()
    VkResult createDescriptorPool(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // Before creating actual Descriptor Pool, Vulkan expects Descriptor Pool Size
        VkDescriptorPoolSize vkDescriptorPoolSize;
        memset((void *)&vkDescriptorPoolSize, 0, sizeof(VkDescriptorPoolSize));

        vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkDescriptorPoolSize.descriptorCount = 1;

        // Create the Descriptor Pool
        VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo;
        memset((void *)&vkDescriptorPoolCreateInfo, 0, sizeof(VkDescriptorPoolCreateInfo));

        vkDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        vkDescriptorPoolCreateInfo.pNext = NULL;
        vkDescriptorPoolCreateInfo.flags = 0;
        vkDescriptorPoolCreateInfo.poolSizeCount = 1;
        vkDescriptorPoolCreateInfo.pPoolSizes = &vkDescriptorPoolSize;
        vkDescriptorPoolCreateInfo.maxSets = 1;

        vkResult = vkCreateDescriptorPool(vkDevice, &vkDescriptorPoolCreateInfo, NULL, &vkDescriptorPool);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateDescriptorPool() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateDescriptorPool() Succeeded.\n", __func__);
        }

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    // (Ortho Change 7(d)) : Defining createDescriptorSet()
    VkResult createDescriptorSet(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // Initialize Descriptor Set Allocation Info
        VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
        memset((void *)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

        vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        vkDescriptorSetAllocateInfo.pNext = NULL;
        // No .flags member
        vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool; // global
        vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
        vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout; // global

        vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkAllocateDescriptorSets() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkAllocateDescriptorSets() Succeeded.\n", __func__);
        }

        // Describe whether we want a buffer as a uniform or an image as uniform
        VkDescriptorBufferInfo vkDescriptorBufferInfo;
        memset((void *)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

        vkDescriptorBufferInfo.buffer = uniformData.vkBuffer;
        vkDescriptorBufferInfo.offset = 0;
        vkDescriptorBufferInfo.range = sizeof(struct MyUniformData);

        // Now update the above Descriptor Set directly to the Shader
        // There are 2 ways to update - 1) Writing Directly to the shader or 2) Copying from one shader to another shader (like Transformed Feedback)
        // We will prefer writing to the shader - this requires initialization of the following structure
        VkWriteDescriptorSet vkWriteDescriptorSet;
        memset((void *)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.dstSet = vkDescriptorSet;
        vkWriteDescriptorSet.dstArrayElement = 0;
        vkWriteDescriptorSet.descriptorCount = 1;
        vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
        vkWriteDescriptorSet.pImageInfo = NULL;       // will be needed while Texturing
        vkWriteDescriptorSet.pTexelBufferView = NULL; // for texture tiling
        vkWriteDescriptorSet.dstBinding = 0;          // because our uniform is at 0th binding index in the shader

        // ***!!!*** QAATIL FUNCTION ***!!!***
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);

        fprintf(gpFile, "%s() : vkUpdateDescriptorSets() Completed Successfully.\n", __func__);

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    // (Pipeline) Step 2(c) : Defining createPipeline() at this specific position.
    VkResult createPipeline(void)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // code
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (Start)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        // (Pipeline) Step 1 : Done globally ...

        // (Pipeline) Step 2 : Done in initialize() ...

        // (Pipeline) Step 3 : Defining the function ...

        // ********* VERTEX INPUT STATE *********
        VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
        memset((void *)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));

        // Multicolored Triangle Change 5(a)

        // For Vertex Position
        vkVertexInputBindingDescription_array[0].binding = 0; // corressponding to layout(location = 0) in vec4 vPosition; in Vertex Shader
        vkVertexInputBindingDescription_array[0].stride = sizeof(float) * 3;
        vkVertexInputBindingDescription_array[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        // For Vertex Color
        vkVertexInputBindingDescription_array[1].binding = 1; // corressponding to layout(location = 1) in vec4 vColor; in Vertex Shader
        vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 3;
        vkVertexInputBindingDescription_array[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2];
        memset((void *)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));

        // Multicolored Triangle Change 5(b)

        // For Vertex Position
        vkVertexInputAttributeDescription_array[0].binding = 0;
        vkVertexInputAttributeDescription_array[0].location = 0;
        vkVertexInputAttributeDescription_array[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        vkVertexInputAttributeDescription_array[0].offset = 0;

        // For Vertex Color
        vkVertexInputAttributeDescription_array[1].binding = 1;
        vkVertexInputAttributeDescription_array[1].location = 1;
        vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        vkVertexInputAttributeDescription_array[1].offset = 0;

        VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
        memset((void *)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

        vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vkPipelineVertexInputStateCreateInfo.pNext = NULL;
        vkPipelineVertexInputStateCreateInfo.flags = 0;
        vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = _ARRAYSIZE(vkVertexInputBindingDescription_array);
        vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vkVertexInputBindingDescription_array;
        vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
        vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vkVertexInputAttributeDescription_array;

        // ********* INPUT ASSEMBLY STATE *********
        VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo;
        memset((void *)&vkPipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

        vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        vkPipelineInputAssemblyStateCreateInfo.pNext = NULL;
        vkPipelineInputAssemblyStateCreateInfo.flags = 0;
        vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        // vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable => used in Geometry Shader & indexed drawing

        // ********* RASTERIZER STATE *********
        VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo;
        memset((void *)&vkPipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

        vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        vkPipelineRasterizationStateCreateInfo.pNext = NULL;
        vkPipelineRasterizationStateCreateInfo.flags = 0;
        vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // Corrected Y - Perspective Matrix Way - Change 1
        vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;                            // gives validation error if explicit initialization of this member is skipped

        // ********* COLOR BLEND STATE *********
        VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState_array[1];
        memset((void *)vkPipelineColorBlendAttachmentState_array, 0, sizeof(VkPipelineColorBlendAttachmentState) * _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array));

        vkPipelineColorBlendAttachmentState_array[0].colorWriteMask = 0xf; // do not mask out any color ... *** IF NOT GIVEN EXPLICITLY, TRIANGLE WON'T APPEAR ... ANDHAAAAAAR! ***
        vkPipelineColorBlendAttachmentState_array[0].blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo;
        memset((void *)&vkPipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

        vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        vkPipelineColorBlendStateCreateInfo.pNext = NULL;
        vkPipelineColorBlendStateCreateInfo.flags = 0;
        vkPipelineColorBlendStateCreateInfo.attachmentCount = _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array);
        vkPipelineColorBlendStateCreateInfo.pAttachments = vkPipelineColorBlendAttachmentState_array;

        // ********* VIEWPORT SCISSOR STATE *********
        VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo;
        memset((void *)&vkPipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));

        vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vkPipelineViewportStateCreateInfo.pNext = NULL;
        vkPipelineViewportStateCreateInfo.flags = 0;

        // Viewport
        vkPipelineViewportStateCreateInfo.viewportCount = 1;

        memset((void *)&vkViewport, 0, sizeof(VkViewport)); // declared globally ...
        vkViewport.x = 0;
        vkViewport.y = 0;
        vkViewport.width = vkExtent2D_swapchain.width;
        vkViewport.height = vkExtent2D_swapchain.height;
        vkViewport.minDepth = 0.0f;
        vkViewport.maxDepth = 1.0f;

        vkPipelineViewportStateCreateInfo.pViewports = &vkViewport;

        // Scissor
        vkPipelineViewportStateCreateInfo.scissorCount = 1;

        memset((void *)&vkRect2D_scissor, 0, sizeof(VkRect2D)); // declared globally ...
        vkRect2D_scissor.offset.x = 0;
        vkRect2D_scissor.offset.y = 0;
        vkRect2D_scissor.extent.width = vkExtent2D_swapchain.width;
        vkRect2D_scissor.extent.height = vkExtent2D_swapchain.height;

        vkPipelineViewportStateCreateInfo.pScissors = &vkRect2D_scissor;

        // ********* DEPTH STENCIL STATE *********
        // As we don't have Depth yet, we can omit this state.

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

        // ********* DYNAMIC STATE *********
        // We don't have any Dynamic State.

        // ********* MULTI-SAMPLE STATE *********
        // We need it in Fragment Shader
        VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo;
        memset((void *)&vkPipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

        vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        vkPipelineMultisampleStateCreateInfo.pNext = NULL;
        vkPipelineMultisampleStateCreateInfo.flags = 0;
        vkPipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // validation error if not given explicitly

        // ********* SHADER STAGE STATE *********
        VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2]; // Vertex Shader and Fragment Shader
        memset((void *)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));

        // Vertex Shader
        vkPipelineShaderStageCreateInfo_array[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vkPipelineShaderStageCreateInfo_array[0].pNext = NULL;
        vkPipelineShaderStageCreateInfo_array[0].flags = 0;
        vkPipelineShaderStageCreateInfo_array[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        vkPipelineShaderStageCreateInfo_array[0].module = vkShaderModule_vertex_shader;
        vkPipelineShaderStageCreateInfo_array[0].pName = "main";
        vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL;

        // Fragment Shader
        vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
        vkPipelineShaderStageCreateInfo_array[1].flags = 0;
        vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        vkPipelineShaderStageCreateInfo_array[1].module = vkShaderModule_fragment_shader;
        vkPipelineShaderStageCreateInfo_array[1].pName = "main";
        vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL;

        // ********* TESSELLATION STATE *********
        // We don't have Tessellation Shaders yet, so we can omit this state.

        // As pipeline is created from Pipeline Cache, we will now create Pipeline Cache Object
        VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
        memset((void *)&vkPipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));

        vkPipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        vkPipelineCacheCreateInfo.pNext = NULL;
        vkPipelineCacheCreateInfo.flags = 0;

        VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;
        vkResult = vkCreatePipelineCache(vkDevice, &vkPipelineCacheCreateInfo, NULL, &vkPipelineCache);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreatePipelineCache() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreatePipelineCache() Succeeded.\n", __func__);
        }

        // Now create the actual pipeline ...
        VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo;
        memset((void *)&vkGraphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));

        vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        vkGraphicsPipelineCreateInfo.pNext = NULL;
        vkGraphicsPipelineCreateInfo.flags = 0;
        vkGraphicsPipelineCreateInfo.pVertexInputState = &vkPipelineVertexInputStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pColorBlendState = &vkPipelineColorBlendStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pViewportState = &vkPipelineViewportStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pDepthStencilState = &vkPipelineDepthStencilStateCreateInfo;
        vkGraphicsPipelineCreateInfo.pDynamicState = NULL;
        vkGraphicsPipelineCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
        vkGraphicsPipelineCreateInfo.stageCount = _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array);
        vkGraphicsPipelineCreateInfo.pStages = vkPipelineShaderStageCreateInfo_array;
        vkGraphicsPipelineCreateInfo.pTessellationState = NULL;
        vkGraphicsPipelineCreateInfo.layout = vkPipelineLayout;
        vkGraphicsPipelineCreateInfo.renderPass = vkRenderPass;
        vkGraphicsPipelineCreateInfo.subpass = 0;
        vkGraphicsPipelineCreateInfo.basePipelineHandle = vkPipeline;
        vkGraphicsPipelineCreateInfo.basePipelineIndex = 0;

        vkResult = vkCreateGraphicsPipelines(vkDevice, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, NULL, &vkPipeline);
        if (vkResult != VK_SUCCESS)
        {
            fprintf(gpFile, "%s() : vkCreateGraphicsPipelines() Failed.\n", __func__);
            return (vkResult);
        }

        else
        {
            fprintf(gpFile, "%s() : vkCreateGraphicsPipelines() Succeeded.\n", __func__);
        }

        // We are done with the local Pipeline Cache, so destroy it.
        if (vkPipelineCache)
        {
            vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
            vkPipelineCache = VK_NULL_HANDLE;
            fprintf(gpFile, "%s() : vkDestroyPipelineCache() Succeeded.\n", __func__);
        }

        // (Pipeline) Step 4 : In uninitialize() ...

        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);
        fprintf(gpFile, "===================================\n");
        fprintf(gpFile, "\"%s()\" Log (End)\n", __func__);
        fprintf(gpFile, "===================================\n");
        // INSERT_NEW_LINE_IN_LOG_FILE(gpFile);

        return (vkResult);
    }

    VkResult buildCommandBuffers(VkCommandBuffer &commandBuffer)
    {
        // variable declarations
        VkResult vkResult = VK_SUCCESS;

        // "Final Render"-related code changes (START) ...

        // Bind with the Pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

        //
        //
        // HERE, WE CALL VULKAN RENDERING FUNCTIONS ...
        //
        //

        // (Ortho Change 9) : Bind the Descriptor Set To The Pipeline
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                vkPipelineLayout,
                                0,
                                1,
                                &vkDescriptorSet,
                                0,     // for dynamic shader stages
                                NULL); // for dynamic shader stages

        // Bind Vertex Position Buffer with the Pipeline
        VkDeviceSize vkDeviceSize_offset_position[1];
        memset((void *)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

        vkCmdBindVertexBuffers(commandBuffer,
                               0,                             // index of vertex buffer
                               1,                             // how many vertex buffers
                               &vertexData_position.vkBuffer, // which vertex buffer(s)
                               vkDeviceSize_offset_position); // offsetting within vertex buffers

        // Multicolored Triangle Change 6
        // Bind Vertex Color Buffer with the Pipeline
        VkDeviceSize vkDeviceSize_offset_color[1];
        memset((void *)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

        vkCmdBindVertexBuffers(commandBuffer,
                               1,                          // index of vertex buffer
                               1,                          // how many vertex buffers
                               &vertexData_color.vkBuffer, // which vertex buffer(s)
                               vkDeviceSize_offset_color); // offsetting within vertex buffers

        vkCmdDraw(commandBuffer,
                  6,  // how many vertices to draw ?
                  1,  // how many instances of the geometry to draw ?
                  0,  // from which index to start drawing ?
                  0); // index of geometry instance ?

        return (vkResult);
    }
};
