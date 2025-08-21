#include "SceneEndcredit.h"

SceneEndCredit::SceneEndCredit(/* args */)
{
    // CODE
    quad = new Quad();

    ///////////////////////////// PIPELINE RELATED CODE ////////////////////////////////
    // vkShaderModule_Vertex = ShaderModuleHelper::LoadShaderModule("bin\\shader.vert.spv");
    // vkShaderModule_Fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader.frag.spv");

    // textureQuadPipelineBuilder = new VulkanPipelineBuilder();

    // createPipeline();

    ////////////////////////////////////////////////////////////////////////////////////

    texQuadEndCreditSlide_1 = new TexturedQuad();
    texQuadEndCreditSlide_1->initialize("resources\\textures\\EndCreditSlide_1.png");

    texQuadEndCreditSlide_2 = new TexturedQuad();
    texQuadEndCreditSlide_2->initialize("resources\\textures\\EndCreditSlide_2.png");

    texQuadBirthdaySlide_1 = new TexturedQuad();
    texQuadBirthdaySlide_1->initialize("resources\\textures\\birthday_slide.png");

    fadeInFadeOut = new FadeInFadeOut();
    fadeInFadeOut->alpha = 1.0f;
    sdkCreateTimer(&timer);
}

void SceneEndCredit::createPipeline(void)
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

SceneEndCredit::~SceneEndCredit()
{
    if (fadeInFadeOut)
    {
        delete fadeInFadeOut;
        fadeInFadeOut = nullptr;
    }

    if (texQuadBirthdaySlide_1)
    {
        delete texQuadBirthdaySlide_1;
        texQuadBirthdaySlide_1 = nullptr;
    }
    if (texQuadEndCreditSlide_2)
    {
        delete texQuadEndCreditSlide_2;
        texQuadEndCreditSlide_2 = nullptr;
    }

    if (texQuadEndCreditSlide_1)
    {
        delete texQuadEndCreditSlide_1;
        texQuadEndCreditSlide_1 = nullptr;
    }

    // if (textureQuadPipelineBuilder)
    // {
    //     delete textureQuadPipelineBuilder;
    //     textureQuadPipelineBuilder = nullptr;
    // }

    // ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Vertex);
    // ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Fragment);
}

void SceneEndCredit::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{

    // // BIND WITH THE PIPELINE
    // vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineTexture);

    // // BIND OUR DESCRIPTOR SET TO PIPELINE
    // vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet, 0, NULL);

    // quad->initialCommandBuffer(commandBuffer);
    texQuadEndCreditSlide_1->buildCommandBuffers(commandBuffer);
    texQuadEndCreditSlide_2->buildCommandBuffers(commandBuffer);
    texQuadBirthdaySlide_1->buildCommandBuffers(commandBuffer);

    if (fadeInFadeOut)
        fadeInFadeOut->initialCommandBuffer(commandBuffer);
}

void SceneEndCredit::update(void)
{
    MVP_UniformData mvp_UniformData;

    if (!timer->isRunning())
        sdkStartTimer(&timer);
    elapsed_time = sdkGetTimerValue(&timer);
    elapsed_time = elapsed_time / 1000.0f;

    delta_time = elapsed_time - prev_time;
    prev_time = elapsed_time;

    // printf("SceneEndCredit::update : %f\n", elapsed_time);
    if (elapsed_time >= TSM::SCENE_ENDCREDITS_TIME)
    {
        // printf("completed : %f\n", elapsed_time);
        completed = true;
    }

    const float yAxis_start_point = -12.0f;
    static float xPos = 0.0f;
    static float yPos = yAxis_start_point;

    if (yPos < -yAxis_start_point)
    {
        yPos += 0.5f * delta_time;
        // yPos += 0.0001f;
        // printf("SceneEndCredit::update : %f\n", yPos);
    }

    if (texQuadBirthdaySlide_1)
    {
        memset((void *)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos + 8.0f, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadBirthdaySlide_1->update(mvp_UniformData);
    }

    if (texQuadEndCreditSlide_1)
    {
        memset((void *)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos + 4.0f, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadEndCreditSlide_1->update(mvp_UniformData);
    }

    if (texQuadEndCreditSlide_2)
    {
        memset((void *)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadEndCreditSlide_2->update(mvp_UniformData);
    }

    if (fadeInFadeOut && elapsed_time >= TSM::SCENE_ENDCREDITS_TIME - 5.0f)
    {
        fadeInFadeOut->display();

        if (fadeInFadeOut->alpha < 0.95f)
            fadeInFadeOut->alpha = fadeInFadeOut->alpha + (0.2 * delta_time);
        else
            fadeInFadeOut->alpha = 0.95f;
        // printf("fadeInFadeOut->alpha %f", fadeInFadeOut->alpha);
    }
    else if (fadeInFadeOut)
    {
        fadeInFadeOut->display();

        if (fadeInFadeOut->alpha >= 0.0f)
            fadeInFadeOut->alpha = fadeInFadeOut->alpha - (0.2 * delta_time);
        // fadeInFadeOut->update();
    }
}

void SceneEndCredit::onResize(int width, int height)
{

    // if (vkPipelineTexture)
    // {
    //     vkDestroyPipeline(vkDevice, vkPipelineTexture, NULL);
    //     vkPipelineTexture = VK_NULL_HANDLE;
    // }

    // createPipeline();

    texQuadEndCreditSlide_1->resize(width, height);

    texQuadEndCreditSlide_2->resize(width, height);

    texQuadBirthdaySlide_1->resize(width, height);
}
