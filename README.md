# Derydoca Engine
This is a personal project of mine available for anyone who is interested in it. Feel free to fork the project and customize it for your needs. I am in the process of refactoring the engine and as a result, many things may be broken. If you are interested in a stable version, check out the `releases/0.0.1` branch. An older video of that branch is below:

[![Watch the demo video](https://user-images.githubusercontent.com/3605996/53131394-8b5f9200-3521-11e9-8caa-9ff2e4ac5e88.png)](https://youtu.be/E6ZjRsHbDro)

And if you are interested in some of the blog posts I have made about some of the development of this engine, check out the link below:

http://derydoca.com/category/derydoca-game-engine/

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

## Third Party Libraries
*  GLEW (2.1.0) - For exposing OpenGL functions
*  SDL2 (2.0.7) - For window management
*  ASSIMP (3.1.1) - For importing 3D art assets
*  GLM (0.9.8.5) - For all the complex math made to make games
*  stb_image - For image loading
*  cereal - For configuration files and serialized objects
