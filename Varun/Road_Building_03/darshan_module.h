#ifndef DARSHAN_MODULES_HEADER
#define DARSHAN_MODULES_HEADER

VkResult createVertexBuffer_by_darshan(void);
VkResult createUniformBuffer_by_darshan(void);
VkResult updateUniformBuffer_by_darshan(void);

void buildCommandBuffers_by_darshan(uint32_t index);


//////////////// START SHAPES  ////////////////


VkResult createVertexBuffer_2(void);
VkResult createUniformBuffer_2(void);
VkResult buildCommandBuffers_2(uint32_t index_);
VkResult updateUniformBuffer_2(void);

////////////////////////////////////////////////




#endif // !DARSHAN_MODULES_HEADER
