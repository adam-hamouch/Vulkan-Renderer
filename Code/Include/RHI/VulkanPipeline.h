#pragma once

#include "IPipeline.h"

class VulkanPipeline : public IPipeline
{
public:
    void Create(PipelineDesc desc) override;
    void Destroy() override;
};
