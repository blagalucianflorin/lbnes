# lbnes

A Nintendo NES emulator in the works.

## Requirements, Clone & Build


### Linux (Debian)
```bash
sudo apt install cmake g++ libsdl2-dev
git clone https://github.com/blagalucianflorin/lbnes
cd lbnes
mkdir bin && cd bin
cmake .. && make
./lbnes <path/to/rom.nes>
```


### Windows (with MinGW)
Install [CMake](https://cmake.org/download/) and [MinGW](https://sourceforge.net/projects/mingw/) and add them to PATH. CMake can be added to path through its install and MinGW will have to be added manually (probably "C:\MinGW\bin").

Download the latest [SDL2](https://github.com/libsdl-org/SDL/releases) development VC release and extract it somewhere. Add the extracted folder as an environment variable called SDL_PATH.

Clone this repository to a folder and ```cd``` that folder in cmd.
```bash
mkdir bin && cd bin
cmake .. -G "MinGW Makefiles"
mingw32-make
lbnes <path/to/rom.nes>
```


### Windows (with CLion)
Download the latest [SDL2](https://github.com/libsdl-org/SDL/releases) development VC release and extract it somewhere. Add the extracted folder as an environment variable called SDL_PATH.

Clone this repository to a folder and open it in [CLion](https://www.jetbrains.com/clion/download/#section=windows).

You should be able to build and run the project normally.

### macOS

Not supported yet.
