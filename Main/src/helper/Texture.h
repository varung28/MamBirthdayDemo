#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.h"
#include <string>

class Texture
{
    private:
        VkImage vkImage_texture;
        VkDeviceMemory vkDeviceMemory_texture;
        VkImageView vkImageView_texture;
        VkSampler vkSampler_texture;

    public:
        Texture();
        ~Texture();

        VkResult createTexture(std::string filePath);
        VkImageView getImageView();
        VkSampler getSampler();
};

#endif  // TEXTURE_H
