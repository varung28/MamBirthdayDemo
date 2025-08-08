#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{
    sceneEndCredit = new SceneEndCredit();
}

SceneMain::~SceneMain()
{
}

void SceneMain::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    sceneEndCredit->initialCommandBuffer(commandBuffer);
}

void SceneMain::update(void)
{
}
