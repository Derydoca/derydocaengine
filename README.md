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

You will also need to generate the solution and project files. This repo is utilizing CMake for project generation. It has only been tested to be working by generating Visual Studio project files, but you may also try other generators at your own risk. This project will currently only build in debug mode due to the explicit dependency to the glew32 library. This is planned to be fixed in the future when the renderer is upgraded to DX12.
