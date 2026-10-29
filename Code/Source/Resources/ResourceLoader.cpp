#include "Resources/ResourceLoader.h"

#include "RHI/ITexture.h"
#include "RHI/IModel.h"

// Emballe la ressource avant de la charger : si le chargement lance une exception, elle est quand meme rendue a la RHI
template<typename T>
static std::shared_ptr<T> MakeShared(T* resource, IRHI* rhi)
{
    return std::shared_ptr<T>(resource, [rhi](T* r) { rhi->Destroy(r); });
}

std::shared_ptr<ITexture> Loader<ITexture>::Load(const char* filePath, const LoadingContext& context)
{
    std::shared_ptr<ITexture> texture = MakeShared(context.rhi->CreateTexture(), context.rhi);
    texture->Load(filePath, context.device);
    return texture;
}

std::shared_ptr<IModel> Loader<IModel>::Load(const char* filePath, const LoadingContext& context)
{
    std::shared_ptr<IModel> model = MakeShared(context.rhi->CreateModel(), context.rhi);
    model->Load(filePath);
    return model;
}

std::shared_ptr<IShader> Loader<IShader>::Load(const char* filePath, const LoadingContext& context, ShaderStage stage)
{
    std::shared_ptr<IShader> shader = MakeShared(context.rhi->CreateShader(), context.rhi);
    shader->Create({ filePath, stage }, context.device);
    return shader;
}
