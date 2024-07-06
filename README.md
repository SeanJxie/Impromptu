# Impromptu

This is my personal CPU-based 3D raster grpahics engine that implements a minimal 3D graphics API. The intent of this project is to give me a better understanding of what implementations of graphics APIs such as OpenGL, Vulkan, and Direct3D do under the hood.

The only required external library is [SDL2](https://github.com/libsdl-org/SDL/releases) (for window and user input management).

By default, Impromptu uses perspective projection with first-person camera controls.

# Building and Running

I build the project with `gcc` on Windows. The build command I use in development looks something like

```
gcc -pedantic -Wall -Werror -fgnu89-inline -std=c99 ^
main.c engine.c model.c vector3.c matrix4.c^
-I[Path to SDL2 includes] ^
-L[Path to SDL2 libraries] ^
-lSDL2 -lSDL2main -lmingw32 ^
-o impromptu.exe 
```

When running the program, make sure `SDL2.dll` (can be found in the SDL2 link above) is in the same directory as the output executable.

# Resources
In no particular order, here are some online resources I found helpful along the way:

|URL|Description|
|-|-|
|https://www.songho.ca/opengl|Detailed (occasionally math-heavy) explanations of OpenGL concepts. |
|https://learnopengl.com|General overview of OpenGL concepts.|
|https://www.pbr-book.org/4ed|Although this book is on physically based rendering of images via raytracing, it is very rigorous in it's explanations of coordinate spaces and transformations for 3D rendering in general.|
|https://computergraphics.stackexchange.com|A forum with expert answers for all things computer graphics.|
|https://github.com/SeanJxie/3d-engine-from-scratch/tree/main|An old project of mine. I had next to no linear algebra experience. Everything's a mess. Impromptu is a reattempt of what I had desired to build back then.|
|https://www.youtube.com/watch?v=ih20l3pJoeU|Tutorial I followed for the project above|
|https://stackoverflow.com/questions/4124041/is-opengl-coordinate-system-left-handed-or-right-handed|SO question about OpenGL's coordinate handedness. Impromptu uses only left-handed coordinate systems.|
|https://en.wikipedia.org/wiki/Z-buffering|The Z-culling section explains how the reverse "painter's algorithm" can be used to optimize Z-culling.|
|https://www.khronos.org/opengl/wiki/Depth_Buffer_Precision and https://developer.nvidia.com/content/depth-precision-visualized|OpenGL wiki and Nvidia article on depth buffer imprecision (and suggestions to improve precision).|
|https://bruop.github.io/frustum_culling/|Article on frustrum culling.|
|https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html|Article on triangle clipping.|
|https://learnwebgl.brown37.net/08_projections/projections_perspective.html|Nice interactive demonstration and derivation of the perspective projection matrix.|
|http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/|First-person controls.|
|https://www.pbr-book.org/4ed/Geometry_and_Transformations/Applying_Transformations#Normals and https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals.html and https://stackoverflow.com/questions/13654401/why-transform-normals-with-the-transpose-of-the-inverse-of-the-modelview-matrix|Text on why normals can not be treated as direction vectors when applying transformations.|
|https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview|OpenGL wiki on the rendering pipeline. Impromptu aims to perform small vital stages of the pipeline.|
|https://en.wikipedia.org/wiki/Back-face_culling|Rundown on back-face culling|
|https://paulbourke.net/dataformats/obj/ and https://paulbourke.net/dataformats/mtl/|OBJ and MTL file specs|
|https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html|A triangle rastization algorithm.|
|https://docs.safe.com/fme/html/FME-Form-Documentation/FME-ReadersWriters/!FME_Geometry/Texture_Coordinates.htm|Rundown on texture coordinates with some nice references.|
|https://computergraphics.stackexchange.com/questions/8515/how-is-lighting-done-in-rasterization-based-pipeline|Into question on lighting in in the context of rasterization.|
|https://en.wikipedia.org/wiki/Computer_graphics_lighting#:~:text=In%20computer%20graphics%2C%20the%20overall,diffuse%2C%20ambient%2C%20and%20specular.|General graphics lighting terminology|
|https://learnwebgl.brown37.net/09_lights/lights_diffuse.html#diffuse-lighting|This and following chapters provide vertex and fragement shaders implementing diffuse/specular/ambient lighting|