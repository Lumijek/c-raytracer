# c-raytracer

Ray tracer based on Ray Tracing in One Weekend. Built to learn programming in C

To run go to the root directory and type:
`gcc main.c utils/*.c glad.c gfx/shader.c -o main -ffast-math -O3 -flto -Iinclude -framework Cocoa -framework OpenGl -framework IOKit -lglfw3 -g
`

... It does look horrible to type however I couldn't get cmake working with the optimization flags and it became very slow. Will update if I can make it work.
Run the program with `./main > image.ppm` to generate an image of the scene. You can also view the image creation real time in openGL (After like 3 hours of dumb bug fixing)
