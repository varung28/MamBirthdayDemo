#include "../../helper/Common.h"
#include "../../helper/Geometry.h"

class SceneIntro
{
private:
    /* data */
public:
    SceneIntro(/* args */);
    ~SceneIntro();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void update(void)
    {
        elapsed_time = sdkGetTimerValue(&timer);
        elapsed_time = elapsed_time / 1000.0f;

        if (elapsed_time >= TSM::SCENE_INTRO_TIME)
        {
            completed = true;
        }
    }
    inline bool isCompleted(void)
    {
        return completed;
    };

    StopWatchInterface *timer = NULL;
    double elapsed_time = 0.0f;
    bool completed = false;
};
