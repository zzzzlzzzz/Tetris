CC=g++
CFLAGS=-c -Wall -std=c++11
CLIBS=-lsfml-graphics -lsfml-window -lsfml-system

all: tetris

tetris: main.o game.o primitive.o menu.o
	$(CC) main.o game.o primitive.o menu.o $(CLIBS) -o ./Tetris/tetris

clean:
	rm -rf *.o

main.o: ./Tetris/main.cpp
	$(CC) $(CFLAGS) ./Tetris/main.cpp

game.o: ./Tetris/game.cpp
	$(CC) $(CFLAGS) ./Tetris/game.cpp

primitive.o: ./Tetris/primitive.cpp
	$(CC) $(CFLAGS) ./Tetris/primitive.cpp

menu.o: ./Tetris/menu.cpp
	$(CC) $(CFLAGS) ./Tetris/menu.cpp
