# 👻👻 PacMaiden 👻👻
PacMaiden é um Remake de PacMan em RayLib. Foi desenvolvido para o projeto final de Programação II dos contribuidores do projeto. O objetivo didático do projeto era de praticar.


## Compilar programa

### Windows
#### Download RayLib
Baixe o instalador MSYS2 (https://www.msys2.org/)
Abra MSYS2 e execute
```shell
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-raylib
```
#### Compilar e rodar
```shell
gcc main.c -o main -lraylib -lwinmm -lgdi32 -lopengl32
./teste_raylib.exe
```

### Linux
#### Download RayLib
Depende da distribuição. Para Ubuntu/Debian e derivados:
```shell
sudo apt update
sudo apt install build-essential
sudo apt install libraylib-dev
```
Para Arch e derivados
```
pacman -Syu raylib
```

#### Compilar e Rodar
```shell
gcc main.c -o main -lraylib -lm -lpthread -ldl -lrt -lX11
```