#pragma once
#include <type_traits>

#include "ResourceCache.h"
#include "ResourceLoader.h"

class ResourcesManager
{
public:
    ResourcesManager(IRHI* rhi, IDevice* device);
    ~ResourcesManager();

    template<typename T, typename... Args>
    std::shared_ptr<T> Load(const char* path, Args&&... args)
    {
        ResourceCache<T>& cache = GetCache<T>();
        if (auto res = cache.Find(path)) 
            return res;

        auto res = Loader<T>::Load(path, context, std::forward<Args>(args)...);
        cache.Add(path, res);
        return res;
    }

    void UnloadUnused();
    void Clear();

private:
    template<typename T>
    ResourceCache<T>& GetCache()
    {
        if constexpr (std::is_same_v<T, ITexture>) return textures;
        else if constexpr (std::is_same_v<T, IShader>) return shaders;
        else if constexpr (std::is_same_v<T, IModel>) return models;
        else static_assert(sizeof(T) == 0, "Type de ressource non gere par ResourcesManager");
    }

    LoadingContext context;

    ResourceCache<ITexture> textures;
    ResourceCache<IShader>  shaders;
    ResourceCache<IModel>   models;
};
