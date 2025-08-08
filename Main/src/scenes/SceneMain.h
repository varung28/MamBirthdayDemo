#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "../helper/Common.h"
#include "SceneEndCredit/SceneEndcredit.h"

class SceneMain
{
private:
    /* data */
    SceneEndCredit *sceneEndCredit = nullptr;

public:
    SceneMain(/* args */);
    ~SceneMain();

    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void update(void);
};

#endif // SCENEMAIN_H
