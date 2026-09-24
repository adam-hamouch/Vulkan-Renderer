#pragma once

#include "IShader.h"
#include "../Include/RHI/VulkanUtils.h"
#include "../Include/RHI/VulkanDevice.h"

class IDevice;

class VulkanShader : public IShader
{
public:
    void Create(ShaderDesc desc, IDevice* device) override;
    void Destroy() override;

    VkShaderModule        GetModule()  const { return shaderModule; }
    ShaderStage GetStage() {return stage;}
    VkShaderStageFlagBits GetVkStage() const;

private:
    VkShaderModule        createShaderModule(const std::vector<char>& code);
    std::vector<char>     readFile(const std::string& filename);

    VulkanDevice*         vkDevice     = nullptr;
    VkShaderModule        shaderModule = VK_NULL_HANDLE;
    ShaderStage           stage;
};