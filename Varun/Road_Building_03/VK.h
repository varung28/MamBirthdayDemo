#pragma once

#define VOID_DERIVATIVES_ICON	102


// Vulkan header files
#define VK_USE_PLATFORM_WIN32_KHR	
#include<vulkan/vulkan.h>
// #include"C:\\VulkanSDK\\VulkanSDK\\Include\\vulkan\\vulkan.h"

// glm realted macros and header files
#define GLM_FORCE_RADIANCE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"			// .hpp means Header file .cpp programs
#include "glm/gtc/matrix_transform.hpp" // gtc means  glm texture compression
// #include"C:\\VulkanSDK\\VulkanSDK\\Include\\glm\\glm.hpp"
// #include"C:\\VulkanSDK\\VulkanSDK\\Include\\glm\\gtc\\matrix_transform.hpp"

#include "darshan_module.h"

extern FILE* DRSJ_gpLogFile;

extern VkDevice DRSJ_vkDevice ;
extern VkPhysicalDeviceMemoryProperties DRSJ_vkPhysicalDeviceMemoryProperties;
extern VkCommandBuffer* DRSJ_vkCommandBuffer_array;
extern VkPipelineLayout vkPipelineLayout ;

extern int winWidth ;
extern int winHeight ;

// Vertex Buffer related variable
typedef struct
{
	VkBuffer vkBuffer;
	VkDeviceMemory vkDeviceMemory;
}VertexData;


// Position
extern VertexData vertexData_position_road_border;
extern VertexData vertexData_position_main_road;
extern VertexData vertexData_position_zebra_crossing;

// Color
extern VertexData vertexData_color_road_border;
extern VertexData vertexData_color_main_road;
extern VertexData vertexData_color_zebra_crossing;


// Uniform related declarations
typedef struct
{
	//glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
}MyUniformData;

typedef struct
{
	glm::mat4 modelMatrix;
} PushConstant;

typedef struct
{
	VkBuffer vkBuffer;
	VkDeviceMemory vkDeviceMemory;
}UniformData;

//////////////// START SHAPES  ////////////////

extern FILE* gpFile;

extern VertexData vertexData_position_2;
extern VertexData vertexData_color_2;
extern UniformData uniformData_2;
////////////////////////////////////////////////

extern UniformData uniformData_road_border;
extern UniformData uniformData_main_road;
extern UniformData uniformData_zebra_crossing;

extern PushConstant pushConstant_model_uniformData[4];


