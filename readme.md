# 👻👻 PacMaiden 👻👻
PacMaiden é um Remake de PacMan em RayLib, uma biblioteca de C voltada para o desenvolvimento de jogos. Foi desenvolvido para o projeto final de Programação II de 2025.2. O objetivo didático do projeto é, principalmente, praticar alocação dinâmica de memória.

## Jogabilidade
Se movimente usando as setas do teclado, coletando todas as bolinhas de cada fase sem ser pego pelos fantasmas. O jogo é um arcade infinito.

## Compilar programa

### Windows
#### Download RayLib
* Instale o MSYS2 (https://www.msys2.org/)
Abra-o MSYS2 e execute
```shell
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-raylib
```
#### Compilar e rodar
* Clone o repositório na pasta home do MSYS2
* Vá para o diretório script/src/
* Dentro do MSYS2, execute
```shell
gcc main.c -o main -lraylib -lwinmm -lgdi32 -lopengl32
./main.exe
```
* Alternativamente, você pode executar o arquivo bash runGame, ainda dentro do MSYS2
```shell
./runGame
```

### Linux
#### Download RayLib
Depende da distribuição. 
* Para Ubuntu/Debian e derivados:
```shell
sudo apt update
sudo apt install build-essential
sudo apt install libraylib-dev
```
* Para Arch e derivados
```
pacman -Syu raylib
```

#### Compilar e Rodar
* Vá para o diretório script/src/
```shell
gcc main.c -o main -lraylib -lm -lpthread -ldl -lrt -lX11
./main
```
* Alternativamente, você pode executar o arquivo bash runGame
```shell
./runGame
```