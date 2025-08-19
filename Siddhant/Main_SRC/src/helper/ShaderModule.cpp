#include "ShaderModule.h"

ShaderModuleHelper::ShaderModuleHelper()
{
}

ShaderModuleHelper::~ShaderModuleHelper()
{
}

void ShaderModuleHelper::DestroyShaderModule(VkShaderModule &vkShaderModule)
{
    if (vkShaderModule)
    {
        vkDestroyShaderModule(vkDevice, vkShaderModule, NULL);
        vkShaderModule = VK_NULL_HANDLE;
    }
}

VkShaderModule ShaderModuleHelper::LoadShaderModule(const char *shaderPath)
{
    // VARIABLE DECLARTIONS
    VkShaderModule vkShaderModule = VK_NULL_HANDLE;
    VkResult vkResult = VK_SUCCESS;
    FILE *fp = NULL;
    size_t size;

    // CODE
    fp = fopen(shaderPath, "rb");
    if (fopen == NULL)
    {
        fprintf(gpFile, "%s => fopen() IS FAILED TO OPEN %s.\n", __func__, shaderPath);
        return VK_NULL_HANDLE;
    }
    else
    {
        fprintf(gpFile, "%s => fopen() IS SUCCEEDED TO OPEN %s.\n", __func__, shaderPath);
    }

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    if (size == 0)
    {
        fprintf(gpFile, "%s => ftell() RETURNED SIZE IS ZERO FOR %s.\n", __func__, shaderPath);
    }

    fseek(fp, 0L, SEEK_SET);

    char *shaderData = (char *)malloc(size * sizeof(char));

    size_t rat_val = fread(shaderData, size, 1, fp);
    if (rat_val != 1)
    {
        fprintf(gpFile, "%s => fread() IS FAILED TO READ %s.\n", __func__, shaderPath);
        return VK_NULL_HANDLE;
    }
    else
    {
        fprintf(gpFile, "%s => fread() IS SUCCEEDED TO READ %s.\n", __func__, shaderPath);
    }

    fclose(fp);

    VkShaderModuleCreateInfo vKShaderModuleCreateInfo;
    memset((void *)&vKShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

    vKShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vKShaderModuleCreateInfo.pNext = NULL;
    vKShaderModuleCreateInfo.flags = 0;
    vKShaderModuleCreateInfo.codeSize = size;
    vKShaderModuleCreateInfo.pCode = (uint32_t *)shaderData;

    vkResult = vkCreateShaderModule(vkDevice, &vKShaderModuleCreateInfo, NULL, &vkShaderModule);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gpFile, "%s => vkCreateShaderModule() IS FAILED.\n", __func__);
        return VK_NULL_HANDLE;
    }
    else
    {
        fprintf(gpFile, "%s => vkCreateShaderModule() IS SUCCEEDED.\n", __func__);
    }

    if (shaderData)
    {
        free(shaderData);
        shaderData = NULL;
    }
    fprintf(gpFile, "%s SHADER MODULE IS SUCCEEDED.\n", shaderPath);
    fprintf(gpFile, "===========================================================================================\n");

    return vkShaderModule;
}