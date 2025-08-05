#ifndef DARSHAN_MODULES_HEADER
#define DARSHAN_MODULES_HEADER

VkResult createVertexBuffer_by_darshan(void);
VkResult createUniformBuffer_by_darshan(void);
VkResult updateUniformBuffer_by_darshan(void);

void buildCommandBuffers_by_darshan(uint32_t index);

#endif // !DARSHAN_MODULES_HEADER
