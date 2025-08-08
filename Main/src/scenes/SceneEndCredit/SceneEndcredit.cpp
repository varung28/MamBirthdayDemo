#include "SceneEndcredit.h"

SceneEndCredit::SceneEndCredit(/* args */)
{
    quad = new Quad();

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
}

void SceneEndCredit::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    quad->initialCommandBuffer(commandBuffer);
}