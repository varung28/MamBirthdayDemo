#ifndef GEOMTRY_H
#define GEOMTRY_H

#include <array>

#include "Common.h"

typedef struct
{
    VkBuffer vkBuffer;
    VkDeviceMemory vkDeviceMemory;
} VertexData;

class Pyramid
{
private:
    /* data */
    VertexData vertexData_position;
    VertexData vertexData_color;

public:
    Pyramid(/* args */);
    ~Pyramid();

    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
};

class Quad
{
private:
    /* data */
    VertexData vertexData_position;
    VertexData vertexData_color;
    VertexData vertexData_texcoord;
    bool isTextureEnabled = false;

public:
    Quad();
    Quad(const std::array<float, 18>&, const std::array<float, 24>&);
    Quad(bool isTextureEnabled);
    ~Quad();

    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
};

#endif // GEMETRY_H
