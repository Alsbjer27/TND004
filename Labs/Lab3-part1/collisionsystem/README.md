## TND004 Labs - Collision System

A simulation and rendering of a system of colliding particles

#### Folder structure

- /include: Header files
- /src: Cpp files
- /data: Example data files

#### Setup instructions
Dependencies:
 - [CMake](https://cmake.org/download/) For cross-platform compiler project generation.
 - [Vcpkg](https://github.com/microsoft/vcpkg) For dependency management
 - C++20 Required, e.g. [Visual Studio](https://visualstudio.microsoft.com/downloads/)

1)  Make a project folder, for example 'lab3'

2)  In the project folder:
    * git clone https://github.com/microsoft/vcpkg 

    The folder structure should be like this
    - lab3
        - collisionsystem (all the code)
            + CMakeLists.txt
            + include
            + src
            + data
        - vcpkg
        - build (Added automatically be CMake later)

3)  Open CMake (we recommend using the GUI here), enter the source path to the collisionsystem folder
    and select one of the "presets" ("MSVC 2022", "Xcode", "Ninja", "Unix Makefiles")
    and hit configure. This will build and the dependencies using vcpkg and make them available to 
    the project. Then they will be configured 

4)  Hit Generate and then Open Project to open the project in your IDE.

5) If Visual Studio is used then right-click on Lab3 in the "Solution Explorer" and select "Set as a Startup Project".

6)  Build and run the 'lab3' executable.
