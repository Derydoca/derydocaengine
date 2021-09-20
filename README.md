# Derydoca Engine
The Derydoca Engine is a game engine and personal project of mine I am making available to anyone interested in it. Feel free to fork it and customize it to your needs or just play around in it. For more information, you can visit my personal website where I periodically post blogs about the development of this engine.

http://derydoca.com/category/derydoca-game-engine/

Watch a short demo video of the engine in action by clicking the thumbnail below!

[![Watch the demo video](https://user-images.githubusercontent.com/3605996/53131394-8b5f9200-3521-11e9-8caa-9ff2e4ac5e88.png)](https://youtu.be/E6ZjRsHbDro)

## Screenshots
![Screenshot of shadow mapping](https://user-images.githubusercontent.com/3605996/52811539-38319f00-304a-11e9-9ce3-5e0f16788ba5.png)
![Screenshot of dynamic LOD system](https://user-images.githubusercontent.com/3605996/52811473-0caeb480-304a-11e9-9e41-8a52f4ed8b9a.png)
![Screenshot of bumpmapping](https://user-images.githubusercontent.com/3605996/52811389-ceb19080-3049-11e9-94d6-108df728787e.png)

## How To Build
After cloning the project you will have to restore the project's submodules. Run the two commands below in your console/terminal window:
```
git submodule init
git submodule update
```

You will also need to generate the solution and project files. This repo is utilizing Premake for project generation. It has only been tested to be working by generating Visual Studio project files, but you may also try other generators at your own risk.

Creating the Visual Studio solution and project files are simple. You will need to have Premake5 installed on your machine and run premake in the repo root. If you want to use the generateProjects.bat file, you will have to define an environment variable %PREMAKE_INSTALLATION_DIR% that points to the directory it is installed at on your machine.

If you do not have Visual Studio 2017 installed on your system, you can change "vs2017" to any valid generator. For more information, check out the [official Premake documentation](https://github.com/premake/premake-core/wiki/Using-Premake).

After running the command, open the solution file in the root directory and proceede as usual.

## Command Line Arguments
You can use command line arguments for specifying a particular project and scene to load immediately. Specifying the project is required, however specifying a scene is not. If you do not specify a scene, an empty scene will be loaded and you can use the GUI to locate and load a level of your choosing.

If you do want to jump into a specific level, there is a table below with a list of command line arguments you can use.

Open the solution file included in the root directory. Once open, right-click on the project DerydocaEngine.Editor.UI and open the Properties panel. Under Configuration Properties > Debugging > Command Arguments, enter in the command line arguments for the scene you want to open.

#### Format
-project \[path\\to\\project\\directory\] -scene \[UUID for the \*.derylevel file to load\]

#### Sample Scene Arguments
This repository comes with a sample project which has many scenes and assets to demo the engine. Here are some scenes you can load via command line arguments:

Scene Name | Command Line Arguments
-----------|-----------------------
A Spooky Skeletal Skeletal Animation | -project ..\\..\\exampleProject -scene a643126f-caf7-4fe7-8460-569373a3dd73
Font Rendering | -project ..\\..\\exampleProject -scene ae4980e6-28a9-40f3-a399-37c0f5a5e463
Sprite Rendering | -project ..\\..\\exampleProject -scene 3732a923-1023-4f27-bef9-e0e74360b89f
Simple Shaders | -project ..\\..\\exampleProject -scene 957d1174-982a-4808-bbdc-4d80bf4864d7
Spotlight Shader | -project ..\\..\\exampleProject -scene 5f587048-0b68-430e-9c24-7fca4bc56efc
Cartoon Shader | -project ..\\..\\exampleProject -scene 681aab0a-9245-44d3-ab29-9787fa3f789c
Misc Complex Shaders | -project ..\\..\\exampleProject -scene 378512b4-c86c-4207-aec6-4a66608611eb
Projection Shader | -project ..\\..\\exampleProject -scene b5d9b8f2-1ecb-4c68-b9a7-f4c15210a020
Postprocessing - Edge Detection | -project ..\\..\\exampleProject -scene 21ba96c7-873c-44aa-8843-c2a07dc4d9f0
Postprocessing - Gaussian Blur | -project ..\\..\\exampleProject -scene cd55550e-0a11-45f2-a713-d44cee4c5bc9
Postprocessing - Bloom | -project ..\\..\\exampleProject -scene abc49756-c89f-447e-a0e4-bb5c3ac752de
Superimpose Wireframe | -project ..\\..\\exampleProject -scene 364d7838-9505-4b14-a6c4-3d9f681c68db
Dynamic Geometry Outlines | -project ..\\..\\exampleProject -scene 309dc507-c2c8-435b-a7aa-af9dda77dfa1
Depth Based Dynamic Tesselation | -project ..\\..\\exampleProject -scene 05b3b1c0-e93f-4129-b0f2-d7d1f9b47a20
Superimpose Wireframe | -project ..\\..\\exampleProject -scene 364d7838-9505-4b14-a6c4-3d9f681c68db
Baked Ambient Occlusion & Shadows | -project ..\\..\\exampleProject -scene aa640cff-3845-4e02-a642-4bddb1f3ef80
Disintegration Shader Effect | -project ..\\..\\exampleProject -scene 3e334272-02fc-41d3-b6c4-de359cdab163
GPU Vertex Displacement | -project ..\\..\\exampleProject -scene 3333a164-0c6c-485f-b2a6-15d901140936
Particle Fountain | -project ..\\..\\exampleProject -scene c16d1737-f86d-4675-a79a-49411bccecab
Mesh Instance Particle System | -project ..\\..\\exampleProject -scene 4943cfa2-0a80-486b-8189-728058263d3c
Smoke Particle System | -project ..\\..\\exampleProject -scene 8cf10126-65aa-474e-a553-a6c00f533178

## Third Party Libraries
*  GLEW (2.1.0) - For exposing OpenGL functions
*  SDL2 (2.0.7) - For window management
*  ASSIMP (3.1.1) - For importing 3D art assets
*  GLM (0.9.8.5) - For all the complex math made to make games
*  stb_image - For image loading
*  cereal - For configuration files and serialized objects
