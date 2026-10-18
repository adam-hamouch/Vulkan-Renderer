#pragma once

#include "IModel.h"
#include <functional>
#include <unordered_map>

namespace std {
    template<> struct hash<Vector2D> {
        size_t operator()(Vector2D const& v) const {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };

    template<> struct hash<Vector3D> {
        size_t operator()(Vector3D const& v) const {
            return ((hash<float>()(v.x) ^
                    (hash<float>()(v.y) << 1)) >> 1) ^
                    (hash<float>()(v.z) << 1);
        }
    };

    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<Vector3D>()(vertex.pos) ^
                    (hash<Vector3D>()(vertex.color) << 1)) >> 1) ^
                    (hash<Vector2D>()(vertex.texCoord) << 1);
        }
    };
}

class VulkanModel : public IModel
{
public:
    void Load(const char* path) override;
    void Destroy() override;
    
private :
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
};
