#include "../../helper/Common.h"
#include "../../helper/Geometry.h"
#include "../../helper/ShaderModule.h"
#include "../../helper/PipelineBuilder.h"

class SceneIntro
{
private:
    /* data */
    StopWatchInterface *timer = NULL;
    double elapsed_time = 0.0f;
    bool completed = false;

    Pyramid *pyramid = nullptr;

    VkShaderModule vkShaderModule_Vertex = VK_NULL_HANDLE;
    VkShaderModule vkShaderModule_Fragment = VK_NULL_HANDLE;
    VulkanPipelineBuilder *textureQuadPipelineBuilder = nullptr;
    VkPipeline vkPipelineTexture = VK_NULL_HANDLE;

public:
    SceneIntro(/* args */);
    ~SceneIntro();
    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
    void createPipeline(void);
    void onResize(int width, int height);

    void update(void);
    inline bool isCompleted(void)
    {
        return completed;
    };
};
