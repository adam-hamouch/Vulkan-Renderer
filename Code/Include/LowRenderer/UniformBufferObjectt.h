#pragma once

#include "maths/includes/Math.h"

struct UniformBufferObject
{
    GpuMat4 model;
    GpuMat4 view;
    GpuMat4 proj;
};
