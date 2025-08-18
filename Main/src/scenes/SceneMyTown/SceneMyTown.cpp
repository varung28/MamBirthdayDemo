#include "SceneMyTown.h"

SceneMyTown::SceneMyTown(/* args */)
{
    // VARIABLE DECLARATIONS
    VkResult vkResult = VK_SUCCESS;

    // CODE
    quad = new Quad();

    // ///////////////////////////// PIPELINE RELATED CODE ////////////////////////////////
    vkShaderModule_Vertex = ShaderModuleHelper::LoadShaderModule("bin\\shader.vert.spv");
    vkShaderModule_Fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader_red.frag.spv");

    textureQuadPipelineBuilder = new VulkanPipelineBuilder();

    createPipeline();

    ////////////////////////////////////////////////////////////////////////////////////

    scenePradnya = new ScenePradnya();
    scenePradnya->initialize();

    sdkCreateTimer(&timer);
}

void SceneMyTown::createPipeline(void)
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
    vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 3; // r,g,b
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
    vkVertexInputAttributeDescription_array[1].location = 1;                        // layout(location = 0) (shader line)
    vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32_SFLOAT; // SIGNED FLOAT, 32 BIT
    vkVertexInputAttributeDescription_array[1].offset = 0;                          // USED FOR INTERLEAVED

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
    vkPipelineShaderStageCreateInfo_array[0].module = vkShaderModule_Vertex;
    vkPipelineShaderStageCreateInfo_array[0].pName = "main";
    vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    // FRAGMENT SHADER
    vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
    vkPipelineShaderStageCreateInfo_array[1].flags = 0;
    vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineShaderStageCreateInfo_array[1].module = vkShaderModule_Fragment;
    vkPipelineShaderStageCreateInfo_array[1].pName = "main";
    vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    textureQuadPipelineBuilder->mShaderStages.resize(2);
    textureQuadPipelineBuilder->mShaderStages[0] = vkPipelineShaderStageCreateInfo_array[0];
    textureQuadPipelineBuilder->mShaderStages[1] = vkPipelineShaderStageCreateInfo_array[1];
    textureQuadPipelineBuilder->mVertexInputInfo = vkPipelineVertexInputStateCreateInfo;
    textureQuadPipelineBuilder->mViewport = vkViewport;
    textureQuadPipelineBuilder->mScissor = vkRect2D_Scissor;
    textureQuadPipelineBuilder->mPipelineLayout = vkPipelineLayout;

    vkPipelineTexture = textureQuadPipelineBuilder->BuildPipeline(vkDevice, vkRenderPass);
}

SceneMyTown::~SceneMyTown()
{
    // if (scenePradnya)
    // {
    //     scenePradnya->uninitialize();
    //     delete scenePradnya;
    //     scenePradnya = nullptr;
    // }

    if (quad)
    {
        delete quad;
        quad = nullptr;
        fprintf(gpFile, "%s => Quad Buffer DESTROYED SUCCESSFULLY.\n", __func__);
    }

    if (vkPipelineTexture)
    {
        vkDestroyPipeline(vkDevice, vkPipelineTexture, NULL);
        vkPipelineTexture = VK_NULL_HANDLE;
    }

    if (textureQuadPipelineBuilder)
    {
        delete textureQuadPipelineBuilder;
        textureQuadPipelineBuilder = nullptr;
    }

    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Vertex);
    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Fragment);
}

void SceneMyTown::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{

    // // BIND WITH THE PIPELINE
    //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineTexture);

    // // BIND OUR DESCRIPTOR SET TO PIPELINE
    //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet, 0, NULL);

    // quad->initialCommandBuffer(commandBuffer);

    // scenePradnya->buildCommandBuffers(commandBuffer);

}

void SceneMyTown::update(void)
{
    elapsed_time = sdkGetTimerValue(&timer);
    elapsed_time = elapsed_time / 1000.0f;

    if (elapsed_time >= TSM::SCENE_ENDCREDITS_TIME)
    {
        completed = true;
    }

    // if (scenePradnya)
    // {
    //     scenePradnya->update();
    // }

}

void SceneMyTown::onResize(int width, int height)
{

    if (vkPipelineTexture)
    {
        vkDestroyPipeline(vkDevice, vkPipelineTexture, NULL);
        vkPipelineTexture = VK_NULL_HANDLE;
    }

    createPipeline();

    // if (scenePradnya)
    // {
    //     scenePradnya->resize(width, height);
    // }
    
}
