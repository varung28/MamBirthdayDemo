#ifndef TREES_H
#define TREES_H

//! Vulkan Related Header Files
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

//! GLM Related Macros and Header Files
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TREES_COUNT              1
#define TREES_DRAW_VERTEX_COUNT  81

extern FILE* gpFile;
extern int winWidth, winHeight;

extern VkDevice vkDevice;
extern VkDescriptorSet vkDescriptorSet;
extern VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
extern VkCommandBuffer* vkCommandBuffer_array;
extern VkPipelineLayout vkPipelineLayout;

VkResult createVertexBuffer_trees(void);
VkResult updateUniformBuffer_trees(void);
void buildCommandBuffers_trees(int);
void uninitialize_trees(void);


#endif  // TREES_H
