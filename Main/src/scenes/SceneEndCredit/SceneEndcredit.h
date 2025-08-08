#ifndef SCENEENDCREDITS_H
#define SCENEENDCREDITS_H

#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"

class SceneEndCredit
{
private:
    /* data */
    Quad *quad = nullptr;

    StopWatchInterface *timer = NULL;
    double elapsed_time = 0.0f;
    bool completed = false;

    VkShaderModule vkShaderModule_Vertex = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_Fragment = VK_NULL_HANDLE;
    VulkanPipelineBuilder textureQuadPipeline = nullptr;

public:
    SceneEndCredit(/* args */);
    ~SceneEndCredit();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void update(void)
    {
        elapsed_time = sdkGetTimerValue(&timer);
        elapsed_time = elapsed_time / 1000.0f;

        if (elapsed_time >= TSM::SCENE_ENDCREDITS_TIME)
        {
            completed = true;
        }
    }
    inline bool isCompleted(void)
    {
        return completed;
    };
};

#endif // SCENEENDCREDITS_H
