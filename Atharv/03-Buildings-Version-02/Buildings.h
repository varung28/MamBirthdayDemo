#ifndef BUILDINGS_H
#define BUILDINGS_H

//! Vulkan Related Header Files
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

//! GLM Related Macros and Header Files
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUILDING_COUNT  5

extern FILE* gpFile;
extern int winWidth, winHeight;

extern VkDevice vkDevice;
extern VkDescriptorSet vkDescriptorSet;
extern VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
extern VkCommandBuffer* vkCommandBuffer_array;
extern VkPipelineLayout vkPipelineLayout;

VkResult createVertexBuffer_buildings(void);
VkResult updateUniformBuffer_buildings(void);
void buildCommandBuffers_buildings(int);
void uninitialize_buildings(void);


#endif  // BUILDINGS_H
