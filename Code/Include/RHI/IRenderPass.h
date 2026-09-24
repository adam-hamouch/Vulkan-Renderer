#pragma once
#include <vector>

enum class StoreOp
{
    STORE,
    DONTCARE
};

enum class LoadOp
{
    CLEAR,
    LOAD,
    DONTCARE
};

enum class AttachmentFormat {  RGBA8,  BGRA8,  Depth32,  Depth24Stencil8 };

struct Attachment
{
    AttachmentFormat format;

    LoadOp loadOp;
    StoreOp storeOp;

    bool isDepth;
    bool isResolve;

    int sampleCount;
};

struct SubpassDesc  
{
    std::vector<uint32_t> colorAttachmentIndices; 
    std::vector<uint32_t> inputAttachmentIndices;
    int depthAttachmentIndex = -1;
};

struct RenderPassDesc
{
    std::vector<Attachment> attachments; 
    std::vector<SubpassDesc> subPasses;
};

class IDevice;
class ICommandList;

class IRenderPass
{
public:
    virtual ~IRenderPass() = default;
    
    virtual void Create(IDevice* device, RenderPassDesc desc) = 0;
    virtual void Destroy() = 0;

    virtual void Begin(ICommandList* commandist) = 0;
    virtual void End(ICommandList* commandist) = 0;
};
