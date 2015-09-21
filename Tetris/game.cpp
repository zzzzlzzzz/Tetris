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
			gameMenu("resource\\gamefont.ttf"), fieldWidth(10), fieldHeight(10)
	{
		mainWindow.setFramerateLimit(60);
		//////////////////////////////////////////////////////////////////////////
		if (!backgroundTexture.loadFromFile("resource\\background.png"))
			throw runtime_error("GameSpace::Game::Game !backgroundTexture.loadFromFile(\"resource\\background.png\")");
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.scale(Vector2f(width / 640.0f, height / 480.0f));
		//////////////////////////////////////////////////////////////////////////
		if(!infoFont.loadFromFile("resource\\gamefont.ttf"))
			throw runtime_error("GameSpace::Game::Game !infoFont.loadFromFile(\"resource\\gamefont.ttf\")");

		loseText.setFont(infoFont);
		loseText.setString("Game Over");
		loseText.setCharacterSize(48);
		loseText.setColor(Color(0xCC, 0x00, 0x33));
		loseText.setStyle(sf::Text::Bold);

		pauseText.setFont(infoFont);
		pauseText.setString("Pause");
		pauseText.setCharacterSize(48);
		pauseText.setColor(Color(0xCC, 0x00, 0x33));
		pauseText.setStyle(sf::Text::Bold);
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
						else if (evItem.key.code == Keyboard::Escape)
							manager.setState(GameManager::GameState::MENU);
					}
				}
			}
			else if (manager.getState() == GameManager::GameState::MENU)
			{
				if (gameMenu.doEvent(evItem, mainWindow))
				{
					// очистить поле установить размер и начать игру
					switch (gameMenu.getSize())
					{
					case Menu::FieldSize::MINIMAL:
						fieldWidth = 10;
						fieldHeight = 10;
						break;
					case Menu::FieldSize::MAXIMAL:
						fieldWidth = 20;
						fieldHeight = 20;
						break;
					default:
					case Menu::FieldSize::STANDART:
						fieldWidth = 15;
						fieldHeight = 15;
						break;
					}

					playerBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
					nextBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));

					gameField = Field(fieldWidth, fieldHeight);

					mainView.setCenter(Vector2f(fieldWidth*Primitive::blockSize / 2.0f, fieldHeight*Primitive::blockSize / 2.0f));
					backgroundSprite.setPosition(Vector2f(mainView.getCenter().x - mainView.getSize().x / 2.0f, mainView.getCenter().y - mainView.getSize().y / 2.0f));
					loseText.setPosition(Vector2f(mainView.getCenter().x - (loseText.getLocalBounds().width / 2.0f), mainView.getCenter().y - (loseText.getLocalBounds().height / 2.0f)));
					pauseText.setPosition(Vector2f(mainView.getCenter().x - (pauseText.getLocalBounds().width / 2.0f), mainView.getCenter().y - (pauseText.getLocalBounds().height / 2.0f)));

					manager.setState(GameManager::GameState::PLAY);
				}
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

				if (nextBlock)
					nextBlock->doDrawAt(mainWindow, fieldWidth + 1, 0);
			}
		}
		else if (manager.getState() == GameManager::GameState::MENU)
		{
			gameMenu.doDraw(mainWindow);
		}
		else if (manager.getState() == GameManager::GameState::PAUSE)
		{
			if (playerBlock)
			{
				playerBlock->doDraw(mainWindow);
				gameField.doDraw(mainWindow);

				if (nextBlock)
					nextBlock->doDrawAt(mainWindow, fieldWidth + 1, 0);
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
						playerBlock.swap(nextBlock);
						nextBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
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
/*
		mainView.setCenter(Vector2f(fieldWidth*Primitive::blockSize / 2.0f, fieldHeight*Primitive::blockSize / 2.0f));
		backgroundSprite.setPosition(Vector2f(mainView.getCenter().x - mainView.getSize().x / 2.0f, mainView.getCenter().y - mainView.getSize().y / 2.0f));
		loseText.setPosition(Vector2f(mainView.getCenter().x - (loseText.getLocalBounds().width / 2.0f), mainView.getCenter().y - (loseText.getLocalBounds().height / 2.0f)));
		pauseText.setPosition(Vector2f(mainView.getCenter().x - (pauseText.getLocalBounds().width / 2.0f), mainView.getCenter().y - (pauseText.getLocalBounds().height / 2.0f)));

		playerBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
		gameField = Field(fieldWidth, fieldHeight);*/
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