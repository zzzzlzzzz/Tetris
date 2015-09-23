#ifndef GAME_H
#define GAME_H

#include <random>
#include <chrono>
#include <memory>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "menu.h"
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
		sf::Text statusText;
		sf::Text scoreText;
		sf::Text levelText;
		void doEvent();
		void doDraw();
		void doView();
		void doLogic();
		std::unique_ptr<Primitive> playerBlock;
		std::unique_ptr<Primitive> nextBlock;
		Field gameField;
		enum class GameState{ MENU, PAUSE, PLAY, LOSE, WIN } gameManager;
		Menu gameMenu;
		int fieldWidth, fieldHeight;
		int score;
		int currentLevel;	// текущий уровень
		const int levelUpScore;	// количество очков, после которых будет повышен уровень
		const int maximalLevel;	// максимальный уровень, после которого будет победа
		const int bonusPerLine;	// количество очков за линию
		void updateScore();
		void updateLevel();
		void updateStatus(const std::string& text);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
	public:
		Game(int width, int height, bool fullScreen);
		~Game() = default;
		void doWork();
	};
}

#endif // !GAME_H
