## Overview
A real-time rendering engine built from scratch in C++ and OpenGL, developed incrementally across a series of assignments and consolidated into a standalone engine as the final project.

## Architecture
- Modular manager-based design: dedicated managers for materials, meshes, shaders, and VAOs, decoupling resource loading from rendering logic
- VAO and shader program caching: the VAOManager returns an existing VAO for a given mesh/shader combination when available, and objects sharing a program or VAO reuse the same GPU resource instead of duplicating it, minimizing redundant OpenGL state changes (glUseProgram, glBindVertexArray) during the render loop
- Each renderable object can use an independent shader program when needed
- Centralized input and game-loop management, decoupled from the entry point

## Rendering Pipeline
- Deferred shading pipeline with a switchable forward-rendering path
- Configurable G-Buffer setup via a dedicated FBO abstraction (init/resize/destroy lifecycle)
- Support for multiple lighting shaders, switchable from the UI
<p align = "center">
  <img width="800" height="439" alt="LightingPasses" src="https://github.com/user-attachments/assets/03cb698f-0edb-4b06-bfa7-b9eebad07291" />
</p>

## Scene Management
- Hierarchical scene graphs: objects support parent-child relationships, with local transforms resolved recursively into world-space model matrices each frame
- Multiple scenes definable and switchable at runtime
- Multiple cameras per scene, including both first-person (movement with WASD and rotation with numpad arrows) and orbital (left click) camera controllers
- Aspect-ratio-preserving projection matrix that adapts to window resizing
<p align = "center">
  <img width="799" height="439" alt="SceneManagement" src="https://github.com/user-attachments/assets/3de6c2f1-5a55-4803-916f-e4444cb4485f" />
</p>

## Lighting & Materials
- Full implementation of the Disney 2012 principled BRDF
- Directional and spot lights, with physically based distance attenuation and a window function for spot cutoff
- Distance fog, blending distant objects into the background
- Bump mapping
- Anisotropic texture filtering
- Runtime-tunable light properties
<p align = "center">
  <img width="800" height="452" alt="Disney2012" src="https://github.com/user-attachments/assets/e618029d-a1bc-4303-ac29-8b1d59a93c99" />
</p>

## Post-Processing
- Modular post-processing stack, chainable and configurable per scene:
  - Bloom
  - Depth of Field (driven by the depth buffer, tunable live)
  - Motion blur (tunable live)
  - Sobel
  - Gaussian blur
  - Custom convolution kernels uploadable via uniform variables
<p align = "center">
  <img width="798" height="439" alt="PostProcessing" src="https://github.com/user-attachments/assets/9342b520-d821-44a0-a20a-dd52ced5f24a" />
</p>

## Other features
- Procedural pattern generation on mesh surfaces
- Object motion driven by Catmull-Rom interpolation
- Support for loading external mesh files into the scene
<p align = "center">
  <img width="800" height="442" alt="ObjImp" src="https://github.com/user-attachments/assets/29f1f1d7-ed0a-471b-96d8-b56f2eb510d5" />
</p>

## Requirements
- Visual Studio 2022 (or compatible)
- Windows
