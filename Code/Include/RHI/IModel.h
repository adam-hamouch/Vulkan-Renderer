#pragma once
#include <vector>
#include "../Libs/maths/includes/Math.h"

struct Vertex
{
    Vector3D pos;
    Vector3D color;
    Vector2D texCoord;

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

class IModel
{
public:
    virtual ~IModel() = default;
    
    virtual void Load(const char* path) = 0;
    virtual void Destroy() = 0;

    const std::vector<Vertex>&   GetVertices() const { return vertices; }
    const std::vector<uint32_t>& GetIndices()  const { return indices; }
    
protected:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
