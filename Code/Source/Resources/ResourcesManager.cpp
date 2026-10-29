#include "Resources/ResourcesManager.h"

ResourcesManager::ResourcesManager(IRHI* rhi, IDevice* device)
    : context{ rhi, device }
{
}

ResourcesManager::~ResourcesManager()
{
    Clear();
}

void ResourcesManager::UnloadUnused()
{
    textures.UnloadUnused();
    shaders.UnloadUnused();
    models.UnloadUnused();
}

void ResourcesManager::Clear()
{
    textures.Clear();
    shaders.Clear();
    models.Clear();
}
