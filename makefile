all:
	gcc -Iinclude -Llib -o bin/main src/*.c  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

run:
	./bin/main
