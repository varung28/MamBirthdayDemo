#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{

    TSM::selectedScene = TSM::SCENE::INTRO;

    sceneEndCredit = new SceneEndCredit();
    sceneMyTown = new SceneMyTown();
    sceneIntro = new SceneIntro();
}

SceneMain::~SceneMain()
{
    if (sceneEndCredit)
    {
        delete sceneEndCredit;
        sceneEndCredit = nullptr;
    }

    if (sceneMyTown)
    {
        delete sceneMyTown;
        sceneMyTown = nullptr;
    }

    if (sceneIntro)
    {
        delete sceneIntro;
        sceneIntro = nullptr;
    }
}

void SceneMain::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    switch (TSM::selectedScene)
    {
    case TSM::SCENE::INTRO:
        sceneIntro->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::MAIN:
        sceneMyTown->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::END_CREDITS:
        sceneEndCredit->initialCommandBuffer(commandBuffer);
        break;
    }
}

void SceneMain::update(void)
{
    switch (TSM::selectedScene)
    {
    case TSM::SCENE::INTRO:
        sceneIntro->update();
        break;

    case TSM::SCENE::MAIN:
        sceneMyTown->update();
        break;

    case TSM::SCENE::END_CREDITS:
        sceneEndCredit->update();
        break;
    }
}

void SceneMain::onResize(int width, int height)
{
    if (sceneIntro)
        sceneIntro->onResize(width, height);
    if (sceneMyTown)
        sceneMyTown->onResize(width, height);
    if (sceneEndCredit)
        sceneEndCredit->onResize(width, height);
}
