#ifndef SCENEMYTOWN_H
#define SCENEMYTOWN_H

#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"

#include "pradnya.h"
#include "Buildings.h"
#include "Trees.h"

class SceneMyTown
{
private:
    /* data */
    Quad *quad = nullptr;

    StopWatchInterface *timer = NULL;
    double elapsed_time = 0.0f;
    bool completed = false;

    VkShaderModule vkShaderModule_Vertex = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_Fragment = VK_NULL_HANDLE;
    VulkanPipelineBuilder *textureQuadPipelineBuilder = nullptr;
    VkPipeline vkPipelineTexture = VK_NULL_HANDLE;

    ScenePradnya *scenePradnya = nullptr;
    Buildings *buildings = nullptr;
    Trees* trees = nullptr;

public:
    SceneMyTown(/* args */);
    ~SceneMyTown();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void createPipeline(void);
    void update(void);
    void onResize(int width, int height);

    inline bool isCompleted(void)
    {
        return completed;
    };
};

#endif // SCENEMYTOWN_H
