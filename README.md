# Vulkan Renderer: built on a custom RHI

![Renderer demo](docs/vikingroom.gif)

## Architecture

```mermaid
flowchart TD
    A[main.cpp] -->|runs| B[Application]
    B -->|instantiates| C[RHI interfaces]
    C -->|implemented by| D[Vulkan RHI]
    D --> E[Vulkan backend]
```

## Features

- Render Hardware Interface (RHI) abstraction layer
- Vulkan backend
- Model and texture loading
- MSAA

## Build

**Requirements**

- Windows
- CMake 3.16 or later
- Vulkan SDK 1.3.296.0

> **Note:** assets are loaded from relative paths, so the executable must be launched from its own output directory.

## Credits

- [Vulkan Tutorial](https://vulkan-tutorial.com/)