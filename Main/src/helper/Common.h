#ifndef COMMON_H
#define COMMON_H

// VULKAN HEADERS
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

// GLM MACROS & HEADER FILES
#define GLM_FORCE_RADIANCE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "../../external/glm/glm.hpp"
#include "../../external/glm/gtc/matrix_transform.hpp"

#include "../../external/helper_timer/helper_timer.h"

extern BOOL gbActiveWindow;
extern HWND ghwnd;
extern BOOL gbFullScreen;
extern BOOL bWindowMinimized;
extern FILE *gpFile;

extern int winWidth;
extern int winHeight;

// INSTANCE EXTENSION RELATED VARIABLES
extern uint32_t enabledInstanceExtensionCount;
extern const char *enabledInstanceExtensionNames_Array[3];

// VULKAN INSTANCE
extern VkInstance vkInstance;

// Vulkan Presentation Surface
extern VkSurfaceKHR vkSurfaceKHR;

// VULKAN PHYSICAL DEVICE RELATED
extern VkPhysicalDevice vkPhysicalDevice_selected;
extern uint32_t graphicsQueueFamilyIndex_selected;
extern VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;

extern uint32_t physicalDeviceCount;
extern VkPhysicalDevice *vkPhysicalDevice_array;

// DEVICE EXTENSION RELATED VARIABLES
extern uint32_t enabledDeviceExtensionCount;
extern const char *enabledDeviceExtensionNames_Array[1]; // VK_KHR_SWAPCHAIN_EXTENSION_NAMEextern

// VK DEVICE
extern VkDevice vkDevice;

// DEVICE QUEUE
extern VkQueue vkQueue;

// extern VkDescriptorSetLayout vkDescriptorSetLayout;
extern VkViewport vkViewport;
extern VkRect2D vkRect2D_Scissor;
extern VkExtent2D vkExtent2D_swapchain;
// extern VkPipelineLayout vkPipelineLayout;
extern VkRenderPass vkRenderPass;
// extern VkDescriptorSet vkDescriptorSet;

namespace TSM
{
    enum SCENE
    {
        INTRO = 0,
        MAIN,
        END_CREDITS
    };

    extern double SCENE_INTRO_TIME;
    extern double SCENE_MAIN_TIME;
    extern double SCENE_ENDCREDITS_TIME;
    extern bool E2D_DEMO;

    extern double g_current_scene_time;
    extern double g_delta_time;
    extern unsigned int selectedScene;
}

extern float objX;
extern float objY;
extern float objZ;
extern float objIncrement;

extern float scaleX;
extern float scaleY;
extern float scaleZ;
extern float scaleIncrement;

typedef struct
{
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
} MVP_UniformData;

class SCENE
{
public:
    SCENE()
    {
    }
    ~SCENE()
    {
    }

    virtual bool isCompleted(void) = 0;
};

extern float prev_time ;
extern float delta_time;

#endif // COMMON_H