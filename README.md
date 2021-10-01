# Dear-Michael's-Engine
This is a game Engine I am working on using C++17 and OpenGL, inspired by Unity and Unreal Engine. Current requirements are Windows-64 and Visual Studio.

![showcase image](showcase.jpg?raw=true)

## Usage
* To sample implemented features, you can extract and run the .exe in the zip file. 
* Scripting and code-reloading is not supported. 
  * If you wish to add your own actors/game features you must build from source.

## Motivation
I've worked on many game projects before and want to create a performant, minimal, and modifyable code base to learn more about game engines.
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

Systems
- [X] Actor with Components
- [X] Input
- [X] Play/Edit
- [X] Picking/ Transform Gizmos
- [X] Component Registry
- [X] Scene Serialization
- [X] Resource Management
- [X] Material
- [ ] Physics
- [ ] Build

Editor Panels
- [X] Game Window
- [X] Hierarchy
- [X] Console
- [X] Inspector
- [X] Content Browser

Transforms
- [X] Position, Rotation, Scale
- [X] Parent Child Relationships

Components
- [X] Static Model
- [X] Player Input
- [ ] Camera
- [ ] Simple Shape Colliders
- [ ] Rigidbody Physics

Resources
- [X] Model (obj) - mtl with texture paths needed for textures
- [X] Texture2D (image)
- [X] Shader (glsl with defines)
- Prototypes
  - [X] Mesh
  - [X] Material
  - [ ] Animations

Proto Actors / Components / Properties
- [X] Camera
- [X] Skybox
- [X] 3 lines denoting the world zero point


