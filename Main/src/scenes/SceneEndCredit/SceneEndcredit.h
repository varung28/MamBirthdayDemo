#ifndef SCENEENDCREDITS_H
#define SCENEENDCREDITS_H

#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"
#include "../../helper/TexturedQuad.h"

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
    VulkanPipelineBuilder *textureQuadPipelineBuilder = nullptr;
    VkPipeline vkPipelineTexture = VK_NULL_HANDLE;
    
    TexturedQuad* texQuadEndCreditSlide_1 = nullptr;
    TexturedQuad* texQuadEndCreditSlide_2 = nullptr;

public:
    SceneEndCredit(/* args */);
    ~SceneEndCredit();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void createPipeline(void);
    void update(void);
    void onResize(int width, int height);

    inline bool isCompleted(void)
    {
        return completed;
    };
};

#endif // SCENEENDCREDITS_H
