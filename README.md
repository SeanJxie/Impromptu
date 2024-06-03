# Impromptu

This is my personal CPU-based 3D graphics engine that implements a minimal 3D graphics API. The intent of this project is to give me a better understanding of what popular implementations of specifications such as OpenGL, Vulkan, and Direct3D do under the hood.

As result of this "from-scratch" nature, the only external library used is SDL2 (for window and user input management).

By default, Impromptu implements perspective projection with first-person camera controls.

# Building and Running

I build the project with `gcc` on Windows. The build command I use looks something like:

```
gcc main.c engine.c model.c vector3.c matrix4.c -o out.exe -pedantic -Wall -Werror -fgnu89-inline -std=c99 -lmingw32 -I[Path to SDL2/include] -L[Path to SDL2/lib] -lSDL2main -lSDL2 
```

When running the program, make sure `SDL2.dll` is in the same directory as the output executable.

# Resources
Here are some resources I found helpful along the way:
- https://www.songho.ca/opengl
  -  For detailed (occasionally math-heavy) explanations of OpenGL concepts.
- https://learnopengl.com
  - For a general overview of OpenGL concepts.
- https://www.pbr-book.org/4ed 
  - Although this book is on physically based rendering of images via raytracing, it is very rigorous in it's explanations of coordinate spaces and transformations for 3D rendering in general. 
- https://computergraphics.stackexchange.com
  - A forum with expert answers for all things computer graphics.
- https://github.com/SeanJxie/3d-engine-from-scratch/tree/main
  - An old project of mine. I had next to no linear algebra experience. Everything's a mess. It was based entirely on javidx9's tutorial series:
  https://www.youtube.com/watch?v=ih20l3pJoeU&list=RDCMUC-yuWVUplUJZvieEligKBkA&index=4
  - Impromptu is a reattempt of what I had desired to build back then.
- https://stackoverflow.com/questions/4124041/is-opengl-coordinate-system-left-handed-or-right-handed
  - SO question about OpenGL's coordinate handedness. Impromptu uses only left-handed coordinate systems.
- https://en.wikipedia.org/wiki/Z-buffering
  - The Z-culling section explains how the reverse "painter's algorithm" can be used to optimize Z-culling.
- https://www.khronos.org/opengl/wiki/Depth_Buffer_Precision#Why_is_my_depth_buffer_precision_so_poor.3F and https://developer.nvidia.com/content/depth-precision-visualized
  - Articles (and remedial suggestions) on depth buffer imprecision. 
- https://bruop.github.io/frustum_culling/
  - Article on frustrum culling.
- https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html
  - Article on triangle clipping.
- https://learnwebgl.brown37.net/08_projections/projections_perspective.html#:~:text=A%20perspective%20projection%20defines%20a,parallel%20to%20the%20X%2DY%20plane.
  - Nice interactive demonstration and derivation of the perspective projection matrix.