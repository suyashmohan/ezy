# ezy

ezy is aimed to be a simple and minimal 2D game engine. It won't ever have fancy features. 
ezy only aims to provide basic features such as draw Textures, Font and play audio.
The engine is written in C language and uses OpenGL.

## Libraries Uses

* https://github.com/floooh/sokol (zlib License)
* https://github.com/nothings/stb (MIT License)
* https://github.com/HandmadeMath/Handmade-Math (CC0 1.0 Universal)
* https://github.com/mackron/dr_libs (Public domain or MIT-0)

## How to build

```
git clone --recursive https://github.com/suyashmohan/ezy
cd ezy
cmake -B build
cmake --build build --config Release
```

## How to use

Since ezy aims to be simple, all the libs and game code along with engine code resides in this single repo.
function `frame` in game.c is where the game logic should go. This method is called on every frame.
`start` is called in starting of game and `end` is called at ending of game.

Note: ezy can only play 1 music(mp3) and 1 sound(wav) at a time and only supports 44100Hz and 2 channel files.

## License

MIT License