#include "SceneEndcredit.h"

SceneEndCredit::SceneEndCredit(/* args */)
{
    quad = new Quad();

    vkShaderModule_Vertex = ShaderModuleHelper::LoadShaderModule("bin\\shader.vert.spv");
    vkShaderModule_Fragment = ShaderModuleHelper::LoadShaderModule("bin\\shader.frag.spv");

    sdkCreateTimer(&timer);
}

SceneEndCredit::~SceneEndCredit()
{
    if (quad)
    {
        delete quad;
        quad = nullptr;
        fprintf(gpFile, "%s => Quad Buffer DESTROYED SUCCESSFULLY.\n", __func__);
    }

    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Vertex);
    ShaderModuleHelper::DestroyShaderModule(vkShaderModule_Fragment);
}

void SceneEndCredit::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    quad->initialCommandBuffer(commandBuffer);
}