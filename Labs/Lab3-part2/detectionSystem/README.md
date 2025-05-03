## TND004 Lab3 - Collinear points detection

A rendering system of points and lines

#### Folder 'detectionSystem' structure

- data: Example input data files
	+ output: output files
- linesdiscoverysystem	<-- files with your code go here
	+ include: Header files
	+ src: Cpp files
- renderingSystem
	+ /include: Header files
	+ /src: Cpp files

#### Setup instructions

Dependencies:
 - [CMake](https://cmake.org/download/) For cross-platform compiler project generation
 - [Vcpkg](https://github.com/microsoft/vcpkg) For dependency management
 - C++20 Required, e.g. [Visual Studio](https://visualstudio.microsoft.com/downloads/)
 
1)  Create a new project (lab) folder named, for example, 'lab3-part2' 

2)  In the project folder (e.g. 'lab3-part2'):
	* Unzip the downloaded folder with the code for the lab (part 2)
    * Execute: 'git clone https://github.com/microsoft/vcpkg' in the project folder

    The project folder structure should be like this
    - lab3-part2
		+ detectionSystem
			- data
			- linesdiscoverysystem	<-- files with your code go here
				+ include
				+ src
			- renderingSystem
				+ include
				+ src
			- CMakeLists.txt
		+ build (Added automatically by CMake in the next step)
        + vcpkg
		
3)  Open CMake (we recommend using the GUI here), enter the source path to the 'detectionSystem' parent folder for the *build* folder
    and select one of the "Preset" ("MSVC 2022", "Xcode", "Ninja", "Unix Makefiles") in the CMake GUI
    and hit configure. This will build and the dependencies using vcpkg and make them available to 
    the project. Then they will be configured 

4) Hit Generate and then Open Project to open the project in your IDE.

5) If Visual Studio is used then right-click on 'Rendering' ('LinesDiscovery') in the "Solution Explorer" 
   and select "Set as a Startup Project".

6) Build and run the 'Rendering' ('LinesDiscovery') executable.

