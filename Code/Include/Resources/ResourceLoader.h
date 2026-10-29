#pragma once
#include <memory>

#include "RHI/RHI.h"
#include "RHI/IShader.h"

struct LoadingContext
{
    IRHI*    rhi    = nullptr;
    IDevice* device = nullptr;
};

template<typename T> struct Loader;

template<> struct Loader<ITexture>
{
    static std::shared_ptr<ITexture> Load(const char* filePath, const LoadingContext& context);
};

template<> struct Loader<IModel>
{
    static std::shared_ptr<IModel> Load(const char* filePath, const LoadingContext& context);
};

template<> struct Loader<IShader>
{
    static std::shared_ptr<IShader> Load(const char* filePath, const LoadingContext& context, ShaderStage stage);
};
