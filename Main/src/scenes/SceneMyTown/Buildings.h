#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "../../helper/Common.h"
#include "../../helper/Geometry.h"
#include "../../helper/PipelineBuilder.h"
#include "../../helper/ShaderModule.h"

class Buildings
{
    private:
        const int BUILDING_COUNT = 5;
        const int BUILDING_GEOMETRY_COUNT = 7;
        
        typedef struct
        {
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        } VertexData;

        typedef struct
        {
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatix;
        } VP_UniformData;

        typedef struct
        {
            glm::mat4 modelMatrix;
        } PushData;

        typedef struct
        {
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        } UniformData;

        typedef struct
        {
            VertexData vertexData_position;
            VertexData vertexData_color;
            PushData modelData;
        } BuildingData;

        UniformData uniformData;
        BuildingData buildings[7];

        VkShaderModule vkShaderModule_vertex = VK_NULL_HANDLE;
        VkShaderModule vkShaderModule_fragment = VK_NULL_HANDLE;

    private:
        VkResult __createVertexBuffer(int index, float* position, int positionSize, float* color, int colorSize);
        VkResult createVertexBuffer();

    public:
        Buildings();
        ~Buildings();

        VkResult updateUniformBuffer();
        void buildCommandBuffers(VkCommandBuffer& commandBuffer);
};


#endif  // BUILDINGS_H
