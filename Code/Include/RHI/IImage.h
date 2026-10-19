#pragma once

#include <cstdint>

class IDevice;

enum class Format
{
	Undefined,
	R8_UNORM,
	RGBA8_UNORM,
	RGBA8_SRGB,
	BGRA8_UNORM,
	BGRA8_SRGB,
	RGBA16_FLOAT,
	D32_FLOAT,
	D24_UNORM_S8_UINT,
};

enum class Usage : uint32_t
{
	None            = 0,
	Sampled         = 1 << 0,
	Storage         = 1 << 1,
	ColorAttachment = 1 << 2,
	DepthAttachment = 1 << 3,
	TransferSrc     = 1 << 4,
	TransferDst     = 1 << 5,
};

inline Usage operator|(Usage a, Usage b) { return static_cast<Usage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); }
inline bool  operator&(Usage a, Usage b) { return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0; }

struct ImageDesc
{
	uint32_t width       = 1;
	uint32_t height      = 1;
	uint32_t mipLevels   = 1;
	uint32_t sampleCount = 1;
	Format   format      = Format::RGBA8_UNORM;
	Usage    usage       = Usage::None;
};

class IImage
{
public:
	virtual ~IImage() = default;

	virtual void Create(IDevice* device, const ImageDesc& desc) = 0;
	virtual void Destroy() = 0;
};
