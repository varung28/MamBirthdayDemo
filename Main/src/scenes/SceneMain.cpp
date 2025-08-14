#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{

    // TSM::selectedScene = SCENE::MAIN;
    //  sceneEndCredit = new SceneEndCredit();
    sceneMyTown = new SceneMyTown();
}

SceneMain::~SceneMain()
{
    // if (sceneEndCredit)
    // {
    //     delete sceneEndCredit;
    //     sceneEndCredit = nullptr;
    // }

    if (sceneMyTown)
    {
        delete sceneMyTown;
        sceneMyTown = nullptr;
    }
}

void SceneMain::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    sceneMyTown->initialCommandBuffer(commandBuffer);

    // switch(TSM::selectedScene)
    // {
    //     case SCENE:: :
    //         sceneEndCredit->initialCommandBuffer(commandBuffer);
    //     break;

    //     case TSM::SCENE::
    //     break;

    // }
}

void SceneMain::update(void)
{
}

void SceneMain::onResize(int width, int height)
{
    sceneMyTown->onResize(width, height);
    // sceneEndCredit->onResize(width, height);
}
