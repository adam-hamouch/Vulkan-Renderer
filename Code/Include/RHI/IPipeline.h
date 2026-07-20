#pragma once

class IRenderPass;
class IShader;

enum class PrimitiveTopology
{
    TriangleList,  TriangleStrip,  LineList,  PointList
};

enum class PolygonMode {  Fill,  Wireframe,  Point };

enum class CullMode {  None,  Front,  Back };

enum class VertexFormat 
{
    Float2,  
    Float3,  
    Float4,  
};

struct VertexAttribute 
{
    uint32_t     location;
    VertexFormat format;
    uint32_t     offset;   
};

struct VertexLayout 
{
    std::vector<VertexAttribute> attributes;
    uint32_t                     stride;     
};

struct PipelineDesc
{
    std::vector<IShader> shaders;

    VertexLayout layout;
    PrimitiveTopology topology;
    PolygonMode polygonMode;
    CullMode cullMode;
    
    bool depthTest;
    bool depthWrite;

    bool blendEnable;

    int sampleCount;

    IRenderPass* renderPass = nullptr;
};

class IPipeline
{
public:
    virtual ~IPipeline() = default;
    
    virtual void Create(PipelineDesc desc) = 0;
    virtual void Destroy() = 0;
};
