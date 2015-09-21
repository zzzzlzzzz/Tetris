#include <iostream>
#include <exception>

#include "game.h"

int main(int argc, char* argv[])
{
	try
	{
		GameSpace::Game tetris(640, 480, false);
		tetris.doWork();
	}
	catch (const std::exception& e)
	{
		std::cout << "std::exception " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "... exception" << std::endl;
	}
	return 0;
}