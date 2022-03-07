# dstar_lite
D* Lite path planning with C++ and FreeGlut visualizer


The original paper by Sven Koenig and Maxim Likhachev on the D* Lite algorithm is referenced for the implementation (it can be found [here](http://idm-lab.org/bib/abstracts/papers/aaai02b.pdf))

And also the mdeyo's python implementation of the algorithm is referenced for validty check - [link](https://github.com/mdeyo/d-star-lite)



https://user-images.githubusercontent.com/58758359/156970675-1b05320d-3bcb-47be-b13c-152233911a14.mp4



## 1. Requirements
OpenGL/FreeGlut

The D* lite algorithm itself relies only on the standard built-in libraries. 
For visulization, it requires FreeGlut (Glut is deprecated) supported by OpenGL.
Generally, on the Windows the OpenGL is preinstalled.
You can check if your system has an OpenGL or check of the version info by downloading the GLView from [here](https://realtech-vr.com/home/glview)

The FreeGlut is an open source library for GUI, and you can download it from [here](http://freeglut.sourceforge.net/index.php#download)

For Windows users, it is recommended to download the "Martin Payne's Windows binaries (MSVC and MinGW)" as it already contains .lib file and freeglut header files.


## 2. System setup
I can only provide the FreeGlut setup for Microsoft Visual Studio users on Windows environment.
For other systems, please refer to the FreeGlut website for the setup.

You need to add i) freeglut.h header file and ii) glut.lib/glut32.lib files to the Visual Studio's Auxiliary folder for linking

i) For the header file, the path should be something like
```
C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\VS\include\gl
```
where the last gl folder may exist or not. If it doesn't create the gl folder and simply migrate the freeglut.h there

ii) For the library files, the path should be something like
```
C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\VS\lib
```
There, you can add glut.lib in x64 folder, and glut32.lib in x86 folder

The linking of FreeGlut using CMakeLists' find_package or find_library seem to exhibit erratic behaviours.
So for the MS Visual Studio users (who just need MSVC compiler) on Windows, it's simpler to add this way, and you don't have to link the libaries each time you create a new project this way.


## 3. How to run
i) Download the current project files

ii) On Command Prompt, navigate to the downloaded folder

iii) Make a "build" folder with mkdir build

iv) Run the following (depending on the set up compiler, this can change):
```
$ cmake -G "Visual Studio 17 2022" CMAKE_CXX_COMPILER=g++-4.2 -S ./ -B build
```
v) After the binaries are successfully generated in the build folder, now run the following command to build the project
```
$ cmake --build ./build --config Release
```
vi) In the ./build/Release folder, Dstarlite.exe is generated

vii) Run the executable!


## 4. Parameters to explore

In the "dstar.h", I've selected SWEEPING_RANGE to 5 and MAX_RETRY to 20 in the constructor call.<br /><br />
The sweeping range is essentially the (SWEEPING_RANGE x SWEEPING_RANGE) size of a block with the current position of the robot at a center. This block is scanned for any status change (Placed a new obstacle, Lifted the obstacle)<br />
With the larger sweeping range, the robot can see "farther" and hence make a better judgment but with an increased processing time.


MAX_RETRY is the maximum number of times that robots are allowed to scan within its scan range before giving up.<br />
In some cases, there might be no path to the terminal position because of obstructions. In this case, the stuck_flag_count increments each time the robot rescans until it reaches the MAX_RETRY<br />
Before reaching the MAX_RETRY if there's a new opening that allows navigation to the target position, the robot starts moving again (Note that the new opening should be within the robot's SWEEPING_RANGE otherwise, it can't detect)<br />
