#pragma once

#include "IModel.h"
#include <functional>

class VulkanModel : public IModel
{
public:
    void Load(const char* path) override;
    void Destroy() override;
    
private :
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
};
