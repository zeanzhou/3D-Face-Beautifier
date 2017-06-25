# 3D-Face-Beautifier
## Denpendencies

This project is under development via Visual Studio 2015 and new packages may be added in the future. All library is x64 version.

#### OpenGL
* [GLFW 3.2.1](http://www.glfw.org/download.html)
* [GLEW 2.0.0](http://glew.sourceforge.net)
* [SOIL](http://www.lonesock.net/soil.html) (released on July 7, 2008)
* Assimp 1.40
* GLM

Download the binary package listed above and extract the files into:
    
    3D-Face-Beautifier\dependencies

For SOIL (Simple OpenGL Image Library), your should compile by your self.

    3D-Face-Beautifier\dependencies\SOIL\projects\VC9\SOIL.sln
    
Build it in x64 and Release mode.

For Assimp and GLM, NuGet Package Manager in Visual Studio will automatically install both of them.

#### OpenCV
* [OpenCV 3.2](http://opencv.org/releases.html)

We recommend you install it in

    C:\Program Files\opencv

If you've installed OpenCV in other paths, don't forget to modify Include Path and Library Path in Solution Properties -> VC++ Path.

## File Structure
    .
    ├── 3D_Facial_Beautifier
    │   ├── 3D_Facial_Beautifier.vcxproj
    │   ├── 3D_Facial_Beautifier.vcxproj.filters
    │   ├── 3D_Facial_Beautifier.vcxproj.user
    │   ├── Camera.h
    │   ├── cvtest.cpp
    │   ├── cvtest.h
    │   ├── main.cpp
    │   ├── Mesh.h
    │   ├── Model.h
    │   ├── packages.config
    │   ├── Resources
    │   │   ├── nanosuit
    │   │   └── test.jpg
    │   ├── Shader.h
    │   ├── Shaders
    │   │   ├── obj.frag
    │   │   └── obj.vs
    │   └── x64
    │       └── Debug
    ├── 3D_Facial_Beautifier.sln
    ├── 3D_Facial_Beautifier.VC.db
    ├── 3D_Facial_Beautifier.VC.VC.opendb
    ├── dependencies
    │   ├── glew-2.0.0
    │   │   ├── bin
    │   │   ├── doc
    │   │   ├── include
    │   │   ├── lib
    │   │   └── LICENSE.txt
    │   ├── glew-2.0.0-win32.zip
    │   ├── glfw-3.2.1.bin.WIN64
    │   │   ├── COPYING.txt
    │   │   ├── docs
    │   │   ├── include
    │   │   ├── lib-mingw-w64
    │   │   ├── lib-vc2012
    │   │   ├── lib-vc2013
    │   │   └── lib-vc2015
    │   ├── glfw-3.2.1.bin.WIN64.zip
    │   ├── opencv-3.2.0-vc14.exe
    │   ├── SOIL
    │   │   ├── field_128_cube.dds
    │   │   ├── img_cheryl.jpg
    │   │   ├── img_test.bmp
    │   │   ├── img_test.dds
    │   │   ├── img_test_indexed.tga
    │   │   ├── img_test.png
    │   │   ├── img_test.tga
    │   │   ├── lib
    │   │   ├── projects
    │   │   ├── soil.html
    │   │   ├── src
    │   │   ├── support
    │   │   ├── test_rect.png
    │   │   └── testSOIL.exe
    │   └── soil.zip
    ├── LICENSE
    ├── packages
    │   ├── assimp.v140.3.2
    │   │   ├── assimp.v140.3.2.nupkg
    │   │   └── build
    │   ├── assimp.v140.redist.3.2
    │   │   ├── assimp.v140.redist.3.2.nupkg
    │   │   └── build
    │   └── glm.0.9.8.4
    │       ├── build
    │       └── glm.0.9.8.4.nupkg
    ├── README.md
    └── x64
        └── Debug
            ├── 3D_Facial_Beautifier.exe
            ├── 3D_Facial_Beautifier.pdb
            └── assimp-vc140-mtd.dll
