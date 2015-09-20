#ifndef GAME_H
#define GAME_H

#include <random>
#include <chrono>
#include <memory>

#include <SFML/Graphics.hpp>

#include "primitive.h"

namespace GameSpace
{
	class Game
	{
		std::default_random_engine dre;
		sf::RenderWindow mainWindow;
		sf::View mainView;
		void doEvent();
		void doDraw();
		void doView();
		void doLogic();
		std::unique_ptr<Primitive> playerBlock;
		Field gameField;
	public:
		Game(int width, int height, bool fullScreen);
		~Game() = default;
		void doWork();
	};
}

#endif // !GAME_H
