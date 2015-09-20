#include "game.h"

using namespace std;

using namespace sf;

namespace GameSpace
{
	//////////////////////////////////////////////////////////////////////////
	Game::Game(int width, int height, bool fullScreen)
		:	dre(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count())),
			mainWindow(VideoMode(width, height), "Tetris", fullScreen ? Style::Fullscreen : Style::Default),
			mainView(FloatRect(0, 0, static_cast<float>(width), static_cast<float>(height)))
	{
		mainWindow.setFramerateLimit(60);
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doEvent()
	{
		Event evItem;
		while (mainWindow.pollEvent(evItem))
		{
			if (evItem.type == Event::Closed)
				mainWindow.close();
			else if (evItem.type == Event::KeyPressed)
			{
				if (evItem.key.code == Keyboard::R)
				{
					if (playerBlock)
						playerBlock->rotRight(gameField);
				}
				else if (evItem.key.code == Keyboard::E)
				{
					if (playerBlock)
						playerBlock->rotLeft(gameField);
				}
				else if (evItem.key.code == Keyboard::Right)
				{
					if (playerBlock)
						playerBlock->moveRight(gameField);
				}
				else if (evItem.key.code == Keyboard::Left)
				{
					if (playerBlock)
						playerBlock->moveLeft(gameField);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doDraw()
	{
		if (playerBlock)
		{
			playerBlock->doDraw(mainWindow);
			gameField.doDraw(mainWindow);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doView()
	{
		mainWindow.setView(mainView);
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doLogic()
	{
		if (playerBlock)
		{
			if (playerBlock->doLogic(gameField))
			{
				// есть касание
				if (gameField.isFinal())
				{
					int i = 5;
				}
				else
				{
					playerBlock.reset(getPrimitive(dre, 1, 0));
				}
			}
			gameField.erasing();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doWork()
	{
		playerBlock.reset(getPrimitive(dre, 1, 0));
		gameField = Field(10, 10);

		Clock gameTime;

		while (mainWindow.isOpen())
		{
			if (gameTime.getElapsedTime().asSeconds() > 1 || 
				(gameTime.getElapsedTime().asMilliseconds() > 50 && Keyboard::isKeyPressed(Keyboard::Down)))
			{
				doLogic();
				gameTime.restart();
			}

			doEvent();

			doView();
			mainWindow.clear();
			doDraw();
			mainWindow.display();
		}
	}
	//////////////////////////////////////////////////////////////////////////
}