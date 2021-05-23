# Open-GL-Game-Engine
This is a game Engine I am working on using C++ and OpenGL, inspired by Unity and Unreal Engine. Current requirements are Windows-64 and Visual Studio.

## Motivation
I've worked on many game projects before and want to create a more performant, minimal, and modifyable code base vs what is availiable in commercial engines.
* My goal is not to build a perfect engine, but rather reach the minimal amount of features to support my next game idea.

## Included Dependencies
* ImGui - for building tools and dockable windows
* ImGuizmo - Layer on top of ImGui to draw gizmos such as transform tools
* stb_image.h - Image file library

## External Dependencies (Reccomend to Install through VCPKG for now)
* glad - OpenGL Function Library
* sdl2 - Cross Platform Window Library
* glm - 3D Math Library

## Roadmap
- [X] SDL and OpenGL (glad) integration
- [X] OpenGL Rendering (basic textures and lighting)
- [X] Input
- [X] Actor base class
- [X] Camera class
- [X] Edit and Play Mode
- [X] ImGui Integration - Hierarchy, Console, Inspector (change values)
- [X] Picking (Slow Pixel Based)
- [X] ImGuizmo Integration - Transform Gizmo, Translate, Rotate, Scale
- [ ] Scene Grid Gizmos
- [ ] Component Base Class
- [ ] Static Mesh Component
- [ ] Model Loader and Rendering Refactor
- [ ] Inspector / Hierarchy Improvements - add support for added features
- [ ] Physics System Integration
- [ ] Build Configuration to compile and build without editor tools

## Known Issues
- Currently each Actor takes about 1MB in RAM, this is due to each actor having unique instances of their mesh, model, VAO, VBO, etc.
  - I plan to fix this by managing references via smart pointers with a Mesh Class / Component that handles rendering
- Non Uniform Scaling is unreliable, currently lower priority
- Translating actors with Gizmos outside of view-frustum affects rendering