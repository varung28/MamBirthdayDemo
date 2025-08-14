#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "../../helper/Common.h"
#include "../../helper/Geometry.h"

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
