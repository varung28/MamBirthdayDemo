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

public:
    Quad(/* args */);
    ~Quad();

    void initialCommandBuffer(VkCommandBuffer &commandBuffer);
};
