#include "SceneMyTown.h"

extern bool updateColorAnimation;

SceneMyTown::SceneMyTown(/* args */)
{
    // VARIABLE DECLARATIONS
    VkResult vkResult = VK_SUCCESS;

    // CODE
    // quad = new Quad();

    // // ///////////////////////////// PIPELINE RELATED CODE ////////////////////////////////
    // vkShaderModule_Vertex = ShaderModuleHelper::LoadShaderModule("bin\\shader.vert.spv");
    // vkShaderModule_Fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader_red.frag.spv");

    // createPipeline();

    ////////////////////////////////////////////////////////////////////////////////////

    scenePradnya = new ScenePradnya();
    scenePradnya->initialize();

    scenePradnya_Sky = new ScenePradnya();
    scenePradnya_Sky->initialize();

    buildings = new Buildings();
    buildings->initialize();

    trees = new Trees();
    trees->initialize();

    road = new Road();
    road->initialize();

    streetLight = new StreetLight();
    streetLight->initialize();

    sdkCreateTimer(&timer);
}

void SceneMyTown::createPipeline(void)
{
    // // VERTEX INPUT STATE
    // VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
    // memset((void *)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));

    // // POSITION
    // vkVertexInputBindingDescription_array[0].binding = 0;                // SIMILAR TO GL_ARRAY_BUFFER, CORESPONDING TO LOCATION 0
    // vkVertexInputBindingDescription_array[0].stride = sizeof(float) * 3; // x,y,z
    // vkVertexInputBindingDescription_array[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // // COLOR
    // vkVertexInputBindingDescription_array[1].binding = 1;                // SIMILAR TO GL_ARRAY_BUFFER, CORESPONDING TO LOCATION 1
    // vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 3; // r,g,b
    // vkVertexInputBindingDescription_array[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2]; // RELATED TO SHADER ATTRIBUTE
    // memset((void *)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));

    // // POSITION
    // vkVertexInputAttributeDescription_array[0].binding = 0;
    // vkVertexInputAttributeDescription_array[0].location = 0;                        // layout(location = 0) (shader line)
    // vkVertexInputAttributeDescription_array[0].format = VK_FORMAT_R32G32B32_SFLOAT; // SIGNED FLOAT, 32 BIT
    // vkVertexInputAttributeDescription_array[0].offset = 0;                          // USED FOR INTERLEAVED

    // // COLOR
    // vkVertexInputAttributeDescription_array[1].binding = 1;
    // vkVertexInputAttributeDescription_array[1].location = 1;                        // layout(location = 0) (shader line)
    // vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32_SFLOAT; // SIGNED FLOAT, 32 BIT
    // vkVertexInputAttributeDescription_array[1].offset = 0;                          // USED FOR INTERLEAVED

    // VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
    // memset((void *)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

    // vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // vkPipelineVertexInputStateCreateInfo.pNext = NULL;
    // vkPipelineVertexInputStateCreateInfo.flags = 0;
    // vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = _ARRAYSIZE(vkVertexInputBindingDescription_array);
    // vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vkVertexInputBindingDescription_array;
    // vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
    // vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vkVertexInputAttributeDescription_array;

    // // SHADER STAGE
    // VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2];
    // memset((void *)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));

    // // VERTEX SHADER
    // vkPipelineShaderStageCreateInfo_array[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // vkPipelineShaderStageCreateInfo_array[0].pNext = NULL;
    // vkPipelineShaderStageCreateInfo_array[0].flags = 0;
    // vkPipelineShaderStageCreateInfo_array[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    // vkPipelineShaderStageCreateInfo_array[0].module = vkShaderModule_Vertex;
    // vkPipelineShaderStageCreateInfo_array[0].pName = "main";
    // vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    // // FRAGMENT SHADER
    // vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
    // vkPipelineShaderStageCreateInfo_array[1].flags = 0;
    // vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    // vkPipelineShaderStageCreateInfo_array[1].module = vkShaderModule_Fragment;
    // vkPipelineShaderStageCreateInfo_array[1].pName = "main";
    // vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL; // IF CONSTANTS ARE IN SHADER, SO SHADER COMPILER WILL PRECOMPILE IT, THIS TYPE SHADER IS SPECIALSED & CONSTANTS ARE SPECIALIZED.

    // textureQuadPipelineBuilder->mShaderStages.resize(2);
    // textureQuadPipelineBuilder->mShaderStages[0] = vkPipelineShaderStageCreateInfo_array[0];
    // textureQuadPipelineBuilder->mShaderStages[1] = vkPipelineShaderStageCreateInfo_array[1];
    // textureQuadPipelineBuilder->mVertexInputInfo = vkPipelineVertexInputStateCreateInfo;
    // textureQuadPipelineBuilder->mViewport = vkViewport;
    // textureQuadPipelineBuilder->mScissor = vkRect2D_Scissor;
    // textureQuadPipelineBuilder->mPipelineLayout = vkPipelineLayout;

    // vkPipelineTexture = textureQuadPipelineBuilder->BuildPipeline(vkDevice, vkRenderPass);
}

SceneMyTown::~SceneMyTown()
{
    if (streetLight)
    {
        delete streetLight;
        streetLight = nullptr;
    }

    if (road)
    {
        delete road;
        road = nullptr;
    }

    if (buildings)
    {
        delete buildings;
        buildings = nullptr;
    }

    if (trees)
    {
        delete trees;
        trees = nullptr;
    }

    if (scenePradnya)
    {
        scenePradnya->uninitialize();
        delete scenePradnya;
        scenePradnya = nullptr;
    }

    if (quad)
    {
        delete quad;
        quad = nullptr;
        fprintf(gpFile, "%s => Quad Buffer DESTROYED SUCCESSFULLY.\n", __func__);
    }

    // ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Vertex);
    // ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Fragment);
}

void SceneMyTown::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    // quad->initialCommandBuffer(commandBuffer);

    if (buildings)
        buildings->buildCommandBuffers(commandBuffer);

    if (streetLight)
        streetLight->buildCommandBuffers(commandBuffer);

    if (road)
        road->buildCommandBuffers(commandBuffer);

    if (trees)
        trees->buildCommandBuffers(commandBuffer);

    if (scenePradnya)
        scenePradnya->buildCommandBuffers(commandBuffer);

    if (scenePradnya_Sky)
        scenePradnya_Sky->buildCommandBuffers(commandBuffer);
}

void SceneMyTown::update(void)
{
    if (!timer->isRunning())
        sdkStartTimer(&timer);
    elapsed_time = sdkGetTimerValue(&timer);
    elapsed_time = elapsed_time / 1000.0f;

    delta_time = elapsed_time - prev_time;
    prev_time = elapsed_time;

    printf("SceneMyTown::update : %f\n", elapsed_time);
    // printf("time : %f\n", elapsed_time);
    if (elapsed_time >= TSM::SCENE_MAIN_TIME)
    {
        completed = true;
    }

    if (road)
        road->update();

    if (streetLight)
        streetLight->update();

    if (buildings)
        buildings->update();

    if (trees)
        trees->update();

    if (scenePradnya)
        scenePradnya->update();

    if (scenePradnya_Sky)
        scenePradnya_Sky->update_sky();

    if (elapsed_time >= TSM::SCENE_MAIN_TIME / 2.0)
    {
        if (elapsed_time >= (TSM::SCENE_MAIN_TIME / 2.0) + 5.0f)
            updateColorAnimation = true;
    }
}

void SceneMyTown::onResize(int width, int height)
{
    // createPipeline();

    if (road)
        road->resize(width, height);

    if (streetLight)
        streetLight->resize(width, height);

    if (trees)
        trees->resize(width, height);

    if (buildings)
        buildings->resize(width, height);

    if (scenePradnya_Sky)
        scenePradnya_Sky->resize(width, height);

    if (scenePradnya)
        scenePradnya->resize(width, height);
}
