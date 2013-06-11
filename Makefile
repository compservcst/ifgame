all:
	g++ ifgame.cpp -o ifgame
windows:
	i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ ifgame.cpp -o ifgame.exe
