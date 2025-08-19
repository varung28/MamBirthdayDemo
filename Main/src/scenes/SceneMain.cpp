#include "SceneMain.h"

SceneMain::SceneMain(/* args */)
{

    TSM::selectedScene = TSM::SCENE::INTRO;
    TSM::E2D_DEMO = true; // if want timer based scene swicth

    //fadeInFadeOut = new FadeInFadeOut();
    sceneEndCredit = new SceneEndCredit();
    sceneMyTown = new SceneMyTown();
    sceneIntro = new SceneIntro();

    sdkCreateTimer(&timer);
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
        if (sceneIntro && sceneIntro->isCompleted() == false)
            sceneIntro->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::MAIN:
        if (sceneMyTown && !sceneMyTown->isCompleted())
            sceneMyTown->initialCommandBuffer(commandBuffer);
        break;

    case TSM::SCENE::END_CREDITS:
        if (sceneEndCredit && !sceneEndCredit->isCompleted())
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
        if (sceneIntro)
            sceneIntro->update();

        if (sceneIntro->isCompleted() && TSM::E2D_DEMO)
        {
            TSM::selectedScene = TSM::SCENE::MAIN;
            printf("Updatiiing :\n");
        }
        break;

    case TSM::SCENE::MAIN:
        if (sceneMyTown)
            sceneMyTown->update();

        if (sceneMyTown->isCompleted() && TSM::E2D_DEMO)
            TSM::selectedScene = TSM::SCENE::END_CREDITS;

        break;

    case TSM::SCENE::END_CREDITS:
        if (sceneEndCredit)
            sceneEndCredit->update();

        // if (sceneEndCredit->isCompleted() && TSM::E2D_DEMO)
        //     TSM::selectedScene++;

        break;
    }

    printf("TSM::selectedScene %d\n", TSM::selectedScene);
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
