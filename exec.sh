
rm culminating
gcc main.c dos.o -o culminating  `sdl2-config --libs --cflags` -lGLEW -lGL -lm -lpthread -ffast-math
./culminating