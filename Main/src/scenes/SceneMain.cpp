#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{

    // TSM::selectedScene = TSM::SCENE::MAIN;

    //fadeInFadeOut = new FadeInFadeOut();
    sceneEndCredit = new SceneEndCredit();
    sceneMyTown = new SceneMyTown();
    sceneIntro = new SceneIntro();
}

SceneMain::~SceneMain()
{
    if (fadeInFadeOut) {
        delete fadeInFadeOut;
        fadeInFadeOut = nullptr;
    }

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
        if(sceneIntro)
            sceneIntro->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::MAIN:
        if (sceneMyTown)
            sceneMyTown->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::END_CREDITS:
        if (sceneEndCredit)
            sceneEndCredit->initialCommandBuffer(commandBuffer);
        break;
    }

    // fadeInFadeOut->initialCommandBuffer(commandBuffer);
}

void SceneMain::update(void)
{
    // fadeInFadeOut->display();
    // fadeInFadeOut->update();

    switch (TSM::selectedScene)
    {
    case TSM::SCENE::INTRO:
        if(sceneIntro)
        sceneIntro->update();
        break;

    case TSM::SCENE::MAIN:
        if (sceneMyTown)
            sceneMyTown->update();
        break;

    case TSM::SCENE::END_CREDITS:
        if (sceneEndCredit)
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

    // if (fadeInFadeOut)
    //     fadeInFadeOut->onResize(width, height);
}
