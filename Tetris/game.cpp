#include "game.h"

using namespace std;

using namespace sf;

namespace GameSpace
{
	//////////////////////////////////////////////////////////////////////////
	Game::Game(int width, int height, bool fullScreen)
		:	dre(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count())),
			mainWindow(VideoMode(width, height), "Tetris", fullScreen ? Style::Fullscreen : Style::Default),
			mainView(FloatRect(0, 0, static_cast<float>(width), static_cast<float>(height))),
			fieldWidth(10), fieldHeight(10)
	{
		mainWindow.setFramerateLimit(60);
		//////////////////////////////////////////////////////////////////////////
		backgroundTexture.loadFromFile("resource\\background.png");
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.setPosition(Vector2f(0, 0));
		backgroundSprite.scale(Vector2f(width / 640.0f, height / 480.0f));
		//////////////////////////////////////////////////////////////////////////
		infoFont.loadFromFile("resource\\gamefont.ttf");

		loseText.setFont(infoFont);
		loseText.setString("Game Over");
		loseText.setCharacterSize(48);
		loseText.setColor(Color(0xCC, 0x00, 0x33));
		loseText.setStyle(sf::Text::Bold);
		loseText.setPosition(Vector2f(width / 2.0f - (loseText.getLocalBounds().width / 2.0f), height / 2.0f - (loseText.getLocalBounds().height / 2.0f)));

		pauseText.setFont(infoFont);
		pauseText.setString("Pause");
		pauseText.setCharacterSize(48);
		pauseText.setColor(Color(0xCC, 0x00, 0x33));
		pauseText.setStyle(sf::Text::Bold);
		pauseText.setPosition(Vector2f(width / 2.0f - (pauseText.getLocalBounds().width / 2.0f), height / 2.0f - (pauseText.getLocalBounds().height / 2.0f)));
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doEvent()
	{
		Event evItem;
		while (mainWindow.pollEvent(evItem))
		{
			if (evItem.type == Event::Closed)
				mainWindow.close();

			if (manager.getState() == GameManager::GameState::PLAY)
			{
				if (playerBlock)
				{
					if (evItem.type == Event::KeyPressed)
					{
						if (evItem.key.code == Keyboard::R)
							playerBlock->rotRight(gameField);
						else if (evItem.key.code == Keyboard::E)
							playerBlock->rotLeft(gameField);
						else if (evItem.key.code == Keyboard::Right)
							playerBlock->moveRight(gameField);
						else if (evItem.key.code == Keyboard::Left)
							playerBlock->moveLeft(gameField);
						else if (evItem.key.code == Keyboard::Pause)
							manager.setState(GameManager::GameState::PAUSE);
					}
				}
			}
			else if (manager.getState() == GameManager::GameState::MENU)
			{

			}
			else if (manager.getState() == GameManager::GameState::PAUSE)
			{
				if (evItem.type == Event::KeyPressed && evItem.key.code == Keyboard::Pause)
					manager.setState(GameManager::GameState::PLAY);
			}
			else if (manager.getState() == GameManager::GameState::LOSE)
			{
				if (evItem.type == Event::MouseButtonPressed)
					manager.setState(GameManager::GameState::MENU);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doDraw()
	{
		mainWindow.draw(backgroundSprite);
		if (manager.getState() == GameManager::GameState::PLAY)
		{
			if (playerBlock)
			{
				playerBlock->doDraw(mainWindow);
				gameField.doDraw(mainWindow);
			}
		}
		else if (manager.getState() == GameManager::GameState::MENU)
		{

		}
		else if (manager.getState() == GameManager::GameState::PAUSE)
		{
			if (playerBlock)
			{
				playerBlock->doDraw(mainWindow);
				gameField.doDraw(mainWindow);
			}
			mainWindow.draw(pauseText);
		}
		else if (manager.getState() == GameManager::GameState::LOSE)
		{
			mainWindow.draw(loseText);
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
		if (manager.getState() == GameManager::GameState::PLAY)
		{
			if (playerBlock)
			{
				if (playerBlock->doLogic(gameField))
				{
					if (gameField.isFinal())
					{
						manager.setState(GameManager::GameState::LOSE);
					}
					else
					{
						playerBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
					}
				}
				gameField.erasing();
			}
		}
		else if (manager.getState() == GameManager::GameState::MENU)
		{

		}
		else if (manager.getState() == GameManager::GameState::PAUSE)
		{

		}
		else if (manager.getState() == GameManager::GameState::LOSE)
		{

		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doWork()
	{
		playerBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
		gameField = Field(fieldWidth, fieldHeight);
		/*
		mainWindow.setSize(Vector2u(fieldWidth*Primitive::blockSize, fieldHeight*Primitive::blockSize));
		mainView.setCenter(Vector2f(fieldWidth*Primitive::blockSize / 2.0, fieldHeight*Primitive::blockSize / 2.0));
		mainView.setSize(Vector2f(fieldWidth*Primitive::blockSize, fieldHeight*Primitive::blockSize));
		*/

		Clock gameTime;

		while (mainWindow.isOpen())
		{
			if (gameTime.getElapsedTime().asSeconds() > 1 || 
				(manager.isPlay() && gameTime.getElapsedTime().asMilliseconds() > 50 && Keyboard::isKeyPressed(Keyboard::Down)))
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