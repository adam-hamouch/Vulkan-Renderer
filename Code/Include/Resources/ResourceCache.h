#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

template<typename T>
class ResourceCache
{
public:
    std::shared_ptr<T> Find(const std::string& path) const
    {
        auto it = entries.find(path);
        return it != entries.end() ? it->second : nullptr;
    }

    void Add(const std::string& path, std::shared_ptr<T> resource) { entries[path] = std::move(resource); }

    void UnloadUnused()
    {
        std::erase_if(entries, [](const auto& e) { return e.second.use_count() == 1; });
    }

    void Clear()
    {
        for (const auto& [path, resource] : entries)
            assert(resource.use_count() == 1 && "Ressource encore referencee a la destruction du ResourcesManager");
        entries.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> entries;
};
