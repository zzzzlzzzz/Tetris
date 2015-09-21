#ifndef GAME_H
#define GAME_H

#include <random>
#include <chrono>
#include <memory>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "menu.h"
#include "gamemanager.h"
#include "primitive.h"

namespace GameSpace
{
	class Game
	{
		std::default_random_engine dre;
		sf::RenderWindow mainWindow;
		sf::View mainView;
		sf::Texture backgroundTexture;
		sf::Sprite backgroundSprite;
		sf::Font infoFont;
		sf::Text loseText;
		sf::Text pauseText;
		void doEvent();
		void doDraw();
		void doView();
		void doLogic();
		std::unique_ptr<Primitive> playerBlock;
		std::unique_ptr<Primitive> nextBlock;
		Field gameField;
		GameManager manager;
		Menu gameMenu;
		int fieldWidth, fieldHeight;
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
	public:
		Game(int width, int height, bool fullScreen);
		~Game() = default;
		void doWork();
	};
}

#endif // !GAME_H
