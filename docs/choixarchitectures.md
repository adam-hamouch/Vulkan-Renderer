# Architecture Decisions

## #1 — Introducing an RHI

### Context
Rendering needed to be abstracted. It was impossible to make the renderer more complex while keeping such a rigid structure, with every function stored in one single class. An RHI also makes it easier to add other graphics APIs later.

### Decision
Introduce an RHI, with an implementation based on a Vulkan backend.

### Consequences
This required a huge amount of restructuring. Splitting the code this way was very difficult, but in the end it only had positive consequences.

## #2 — Adding a Vulkan getter to every class

### Context
Vulkan features are often interdependent. It is hard to combine a fully abstract renderer with a complete implementation of the API.

### Decision
I eventually reverted this decision. Instead, I added casts in the code where needed, to avoid breaking the abstraction principle.

### Consequences
The code contains a few too many `static_cast`s, which are not always safe.

## #3 — The RHI controls the lifetime of every object

### Context
To support several graphics APIs, rendering objects must be managed through an interface that is implemented by API-specific subclasses.

### Decision
The RHI creates objects at init and destroys them at cleanup. Most graphics APIs already work this way. It gives clear control over object lifetimes and greatly reduces risks.

### Consequences
Objects cannot be destroyed in the middle of execution. This is not a real problem, unless the RHI keeps handling objects that are not rendering-specific (e.g. models, which can be destroyed at any time).

## #4 — Only the RHI and the resource manager fully own rendering objects

### Context
As mentioned above, some objects depend on each other. For example, a material needs a texture, but it must not fully own it (the texture is created by the resource manager and may be used by many other materials).

### Decision
Object lifetimes stay managed by the RHI or the resource manager. Use `shared_ptr` if needed.

## #5 — Adding an IImage class

### Context
Image creation was implemented in ITexture, and image view creation in the swap chain. This split was inconsistent and created an unwanted dependency: loading a texture required the swap chain to exist.

### Decision
I created an IImage class that brings these two methods together, so that textures, the swap chain or anything else can create their images and image views independently.

### Consequences
Image lifetimes are a bit more complicated to manage.

## #6 — Resource creation: IRHI as the single factory

### Context
The client must create GPU resources (buffers, images, pipelines…) without seeing any Vulkan type. Some implementations need backend-internal dependencies (e.g. an upload context) that RHI interfaces cannot expose.

### Decision
- IRHI is the only factory: `CreateXxx()` returns an empty object.
- The client then initializes the object with `Create(device, desc)` (two-phase initialization).
- The factory, implemented in the backend, injects internal dependencies through the constructor (e.g. `new VulkanBuffer(&uploadContext)`), so they stay invisible to the client.

### Rejected alternatives
- **Factory on IDevice (D3D12, Vulkan, Metal, WebGPU, NVRHI)**: the most common model, where the device owns memory and context. Here, VulkanDevice only handles GPU selection and queues, and IRHI plays that role. With a single GPU, the difference is mostly naming.

### Consequences
- The device is passed to every `Create` even though the factory already knows it (accepted redundancy).
- Supporting several GPUs would require one factory per device.