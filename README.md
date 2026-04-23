[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)
# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

## Requirements:

### 1. Raylib
- #### Quick Install:
```bash
sudo add-apt-repository ppa:texus/raylib
sudo apt update
sudo apt install libraylib5-dev
```
-  #### Build from Source
  
```bash
sudo apt update
sudo apt install build-essential git cmake libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev

git clone https://github.com/raysan5/raylib.git
cd raylib/src/
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

###  2. X11
```bash
sudo apt install xorg
```

##  Cara Compile & Run:

```bash
make clean
make run
```