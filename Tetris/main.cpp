#include "game.h"

int main(int argc, char* argv[])
{
	GameSpace::Game tetris(640, 480, false);
	tetris.doWork();
	return 0;
}