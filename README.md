## Overview
A real-time rendering engine built from scratch in C++ and OpenGL, developed incrementally across a series of university assignments and consolidated into a standalone engine as the final project.

## Rendering Pipeline
- Deferred shading pipeline with a switchable forward-rendering path (toggleable at runtime)
- Configurable G-Buffer setup via a dedicated FBO abstraction (init/resize/destroy lifecycle)
- Support for multiple lighting shaders, switchable live via keyboard

## Scene Management
- Hierarchical scene graphs: objects support parent-child relationships, with local transforms resolved recursively into world-space model matrices each frame
- Multiple scenes definable and switchable at runtime
- Multiple cameras per scene, including both first-person (keyboard) and orbital (mouse) camera controllers
- Aspect-ratio-preserving projection matrix that adapts to window resizing

## Lighting & Materials
- Full implementation of the Disney 2012 principled BRDF
- Directional and spot lights, with physically based distance attenuation and a window function for spot cutoff
- Distance fog, blending distant objects into the background
- Bump mapping
- Anisotropic texture filtering
- Runtime-tunable light properties via keyboard

## Post-Processing
- Modular post-processing stack, chainable and configurable per scene:
  - Bloom
  - Depth of Field (driven by the depth buffer, tunable live)
  - Motion blur (tunable live)
  - Sobel
  - Gaussian blur
  - Custom convolution kernels uploadable via uniform variables

## Architecture
- Modular manager-based design: dedicated managers for materials, meshes, shaders, and VAOs, decoupling resource loading from rendering logic
- VAO and shader program caching: the VAOManager returns an existing VAO for a given mesh/shader combination when available, and objects sharing a program or VAO reuse the same GPU resource instead of duplicating it — minimizing redundant OpenGL state changes (glUseProgram, glBindVertexArray) during the render loop
- Each renderable object can use an independent shader program when needed
- Centralized input and game-loop management, decoupled from the entry point

## Other features
- Procedural pattern generation on mesh surfaces
- Object motion driven by Catmull-Rom interpolation
- Support for loading external mesh files into the scene

## Requirements
> ⚠️ **Build configuration:** This project only runs correctly in **Debug | x86**.
> Other configurations (Release, x64) are currently unsupported and will fail to build or run.
- Visual Studio 2022 (or compatible)
- Windows

## Images
<p align="center">
  <img width="200" height="200" alt="Procedural" src="https://github.com/user-attachments/assets/233764bb-b253-43c4-93e8-3b3854a28a76" />
  <img width="200" height="200" alt="Bump" src="https://github.com/user-attachments/assets/65ded426-39cd-45d7-ae0f-23cd5e0029d1" />
  <img width="200" height="200" alt="Anisotropic" src="https://github.com/user-attachments/assets/1ea52737-152b-484b-96d1-263624852559" />
  <img width="200" height="200" alt="Motion Blur" src="https://github.com/user-attachments/assets/bd256c48-fe74-4dfa-9fb1-098cd82d5537" />
  <img width="200" height="200" alt="Focal" src="https://github.com/user-attachments/assets/45f90ec3-daf0-47cc-b7e4-2e3cdf22ecb0" />
  <img width="200" height="200" alt="Subsurface" src="https://github.com/user-attachments/assets/b3b71f8e-0d9c-45b6-a176-d2faf4afd32c" />
  <img width="200" height="200" alt="Sheen" src="https://github.com/user-attachments/assets/d86adfb9-a48e-4946-b2ec-384c60b8f889" />
  <img width="200" height="200" alt="Clearcoat" src="https://github.com/user-attachments/assets/91b7a186-2f6b-4b67-94e7-77925a08d245" />
  <img width="810" height="430" alt="diff" src="https://github.com/user-attachments/assets/c75e8cd4-c1e2-4e92-9849-fb5ec5b5833b" />
</p>
