# Swordfight
Recreating a game from college

How to run:
1. Download a C++ compiler (If on windows, you can grab g++ from here: https://sourceforge.net/projects/mingw-w64/). If using 64bit libraries make sure to install a 64bit compiler. 
2. Download Make (If on windows, you can download from here: http://gnuwin32.sourceforge.net/packages/make.htm).
3. Download SDL2 (http://libsdl.org/download-2.0.php) and SDL2 Image (http://www.libsdl.org/projects/SDL_image/) and save them both to a folder. Take note of this folder location.
4. Clone this repo. This repo contains submodules that will have to be initialized as well.
5. Open the makefile, update "LIBPATH" and "INCLUDES" with the folder location from step 3.
6. From the makefile directory, run the 'make' command. This will create the binary file to run the game in the bin/debug folder (to delete all binaries and object files, run 'make clean').