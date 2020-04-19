# Blah
Blah is a 3D game engine written in C and using SDL and OpenGL.  It is very old and originally conformed to the C89 ANSI specification.  Back when I started making it I couldn't think of a decent name for it so I named it 'Blah'.  It is also lacking in optimsation for speed and therefore is not particularly quick.  It has been revised over the years to use more recent features of the language.

Why C and not C++?  When I started creating it, I wanted it to work with both Linux and Windows.  C compilers were more standardised than C++ compilers, and also faster.  I also liked the idea of using standard C linking so that it could be used by other languages that support calling of library functions using standard linking (including C++).  Game libraries and such were also written in C (think SDL and OpenGL) and therefore it seemed like a good choice.  I also really like the simplicity of C and it seemed like a good challenge to learn and develop this all myself and in turn, my understanding, rather than use something that was already out there.

It features:
- Basic material properties
- Simple textured polygons
- Support for Lightwave .lwo file format
- Support for TARGA .tga image files
- Collision detecting using bounding spheres
- Customisable runtime behaviour of individual objects through the use of function pointers (akin to virtual functions in C++)
- Simple light sources
- 2D and 3D custom fonts

