#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "../helper/Common.h"
#include "SceneEndCredit/SceneEndcredit.h"
#include "SceneMyTown/SceneMyTown.h"

class SceneMain
{
private:
    /* data */
    SceneEndCredit *sceneEndCredit = nullptr;
    SceneMyTown *sceneMyTown = nullptr;

public:
    SceneMain(/* args */);
    ~SceneMain();

    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void onResize(int width, int height);
    void update(void);
};

#endif // SCENEMAIN_H
