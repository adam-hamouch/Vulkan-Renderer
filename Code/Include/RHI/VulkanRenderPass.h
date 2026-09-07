#pragma once

#include "IRenderPass.h"

class VulkanRenderPass : public IRenderPass
{
public:
    void Create(IDevice* device, RenderPassDesc desc) override;
    void Destroy() override;

    void Begin(ICommandList* commandist) override;
    void End(ICommandList* commandist) override;
};
