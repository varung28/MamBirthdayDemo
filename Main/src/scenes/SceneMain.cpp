#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{
    sceneEndCredit = new SceneEndCredit();
}

SceneMain::~SceneMain()
{
    if (sceneEndCredit)
    {
        delete sceneEndCredit;
        sceneEndCredit = nullptr;
    }
}

void SceneMain::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    sceneEndCredit->initialCommandBuffer(commandBuffer);
}

void SceneMain::update(void)
{
}

void SceneMain::onResize(int width, int height)
{
    sceneEndCredit->onResize(width, height);
}
