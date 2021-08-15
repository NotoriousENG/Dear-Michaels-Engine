# Dear-Michael's-Engine
This is a game Engine I am working on using C++ and OpenGL, inspired by Unity and Unreal Engine. Current requirements are Windows-64 and Visual Studio.

![showcase image](showcase.jpg?raw=true)

## Usage
* To sample implemented features, you can extract and run the .exe in the zip file. 
* Scripting and code-reloading is not supported. 
  * If you wish to add your own actors/game features you must build from source.

## Motivation
I've worked on many game projects before and want to create a more performant, minimal, and modifyable code base vs what is availiable in commercial engines.
* My goal is not to build a perfect engine, but rather reach the minimal amount of features to support my next game idea.

## Included Dependencies
* ImGui - for building tools and dockable windows
* ImGuizmo - Layer on top of ImGui to draw gizmos such as transform tools
* ImGuiFileDialog - ImGui Cross platform file dialog tools
* stb_image.h - Image file library

## External Dependencies (Reccomend to Install through VCPKG for now)
* glad - OpenGL Function Library
* sdl2 - Cross Platform Window Library
* glm - 3D Math Library
* cereal - Fast Serialization Library
* assimp - Multi Format Asset importer for 3D Models

## VCPKG
First, download and bootstrap vcpkg itself; it can be installed anywhere,
but generally we recommend using vcpkg as a submodule for CMake projects,
and installing it globally for Visual Studio projects.
We recommend somewhere like `C:\src\vcpkg` or `C:\dev\vcpkg`,
since otherwise you may run into path issues for some port build systems.

### First Time Setup
```cmd
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
```
In order to use vcpkg with Visual Studio, run the following command (may require administrator elevation):
```cmd
> .\vcpkg\vcpkg integrate install
```
### Usage
To search for libraries, run:
```cmd
> .\vcpkg\vcpkg search package
```

To install x64 libraries, run:

```cmd
> .\vcpkg\vcpkg install package:x64-windows
```

## Roadmap
- [X] SDL and OpenGL (glad) integration
- [X] OpenGL Rendering (basic textures and lighting)
- [X] Input
- [X] Actor base class
- [X] Camera class
- [X] Edit and Play Mode
- [X] ImGui Integration - Hierarchy, Console, Inspector (change values)
- [X] Picking (Slow Pixel Based)
- [X] ImGuizmo Integration - Transform Gizmo, Translate, Rotate, Scale, Local, World, Snapping
- [X] Inspector Prototype - Hierarchy and Inspector are seperate
- [X] Component Base Class
- [X] Static Mesh Component
- [X] Saving/Loading Scenes
- [X] Model Loader and Rendering Refactor - obj files (mtl holds relative paths to textures)
- [X] Inspector / Hierarchy Improvements - Add/Remove Components / Provide a simple component editor for the Model Component
- [X] Serialize Components - It is now possible to go pure Actor-Component and move more tools into the runtime!
- [X] Register Components - Components can be registered to be added at run time!
- [X] Simple Input Manager - Static instance access anywhere, also comes with a cool player controller component
- [X] Standardized Component Inspection - leverage ImGui to get can't believe it's not unity
- [X] Skybox - Simple implementation that makes a huge difference
- [ ] Plan and Develop a New Resource / Content Manager Pipeline
- [ ] Start planning a simple game to help identify missing features
- [ ] Build Configuration to compile and build without editor tools
- [ ] Physics System Integration
