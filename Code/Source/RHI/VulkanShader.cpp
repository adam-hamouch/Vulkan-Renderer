#include "../Include/RHI/VulkanShader.h"

void VulkanShader::Create(ShaderDesc desc, IDevice* device)
{
    vkDevice = static_cast<VulkanDevice*>(device);
    stage    = desc.stage; 
    
    auto code    = readFile(desc.fileName);
    shaderModule = createShaderModule(code);
}

VkShaderStageFlagBits VulkanShader::GetVkStage() const
{
    switch (stage) {
    case ShaderStage::VERTEX:   return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderStage::FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStage::COMPUTE:  return VK_SHADER_STAGE_COMPUTE_BIT;
    default:                    return VK_SHADER_STAGE_VERTEX_BIT;
    }
}

VkShaderModule VulkanShader::createShaderModule(const std::vector<char>& code) 
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(vkDevice->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("échec de la création d'un module shader!");
    }
        
    return shaderModule;
}
    
std::vector<char> VulkanShader::readFile(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) 
    {
        throw std::runtime_error(std::string {"échec de l'ouverture du fichier "} + filename + "!");
    }
        
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
        
    file.close();

    return buffer;
}

void VulkanShader::Destroy()
{
}
