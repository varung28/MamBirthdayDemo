#pragma once

#include "Common.h"

class ShaderModuleHelper
{
private:
    /* data */
public:
    ShaderModuleHelper();
    ~ShaderModuleHelper();

    static VkShaderModule LoadShaderModule(const char *shaderPath);
    static void DestroyShaderModule(VkShaderModule &vkShaderModule);
};
