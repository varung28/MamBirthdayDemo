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

    extern double g_current_scene_time;
    extern double g_delta_time;
    extern unsigned int selectedScene;
}

#endif // COMMON_H