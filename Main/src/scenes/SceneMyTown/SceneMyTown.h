#ifndef SCENEMYTOWN_H
#define SCENEMYTOWN_H

#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"
<<<<<<< HEAD
#include "Buildings.h"
=======
#include "pradnya.h"
>>>>>>> origin/main

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

<<<<<<< HEAD
    Buildings* buildings = nullptr;
=======
    ScenePradnya *scenePradnya = nullptr;
>>>>>>> origin/main

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
