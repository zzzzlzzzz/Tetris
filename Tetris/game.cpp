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
			gameMenu("resource\\gamefont.ttf"), fieldWidth(10), fieldHeight(10), score(0), gameManager(GameState::MENU)
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
		loseText.setStyle(Text::Bold);

		pauseText.setFont(infoFont);
		pauseText.setString("Pause");
		pauseText.setCharacterSize(48);
		pauseText.setColor(Color(0xCC, 0x00, 0x33));
		pauseText.setStyle(Text::Bold);

		scoreText.setFont(infoFont);
		scoreText.setCharacterSize(20);
		scoreText.setColor(Color(0xCC, 0x00, 0x33));
		scoreText.setStyle(Text::Bold);
		updateScore();
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::updateScore()
	{
		scoreText.setString("Score:\n" + to_string(score));
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doEvent()
	{
		Event evItem;
		while (mainWindow.pollEvent(evItem))
		{
			if (evItem.type == Event::Closed)
				mainWindow.close();

			if (gameManager == GameState::PLAY)
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
							gameManager = GameState::PAUSE;
						else if (evItem.key.code == Keyboard::Escape)
							gameManager = GameState::MENU;
					}
				}
			}
			else if (gameManager == GameState::MENU)
			{
				if (gameMenu.doEvent(evItem, mainWindow))
				{
					// очистить поле установить размер и начать игру
					score = 0;
					updateScore();

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

					scoreText.setPosition(Vector2f((fieldWidth + 1.f)*Primitive::blockSize, 4.f * Primitive::blockSize));

					gameManager = GameState::PLAY;
				}
			}
			else if (gameManager == GameState::PAUSE)
			{
				if (evItem.type == Event::KeyPressed && evItem.key.code == Keyboard::Pause)
					gameManager = GameState::PLAY;
			}
			else if (gameManager == GameState::LOSE)
			{
				if (evItem.type == Event::MouseButtonPressed)
					gameManager = GameState::MENU;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doDraw()
	{
		mainWindow.draw(backgroundSprite);
		if (gameManager == GameState::PLAY)
		{
			if (playerBlock)
			{
				playerBlock->doDraw(mainWindow);
				gameField.doDraw(mainWindow);

				if (nextBlock)
					nextBlock->doDrawAt(mainWindow, fieldWidth + 1, 0);

				mainWindow.draw(scoreText);
			}
		}
		else if (gameManager == GameState::MENU)
		{
			gameMenu.doDraw(mainWindow);
		}
		else if (gameManager == GameState::PAUSE)
		{
			if (playerBlock)
			{
				playerBlock->doDraw(mainWindow);
				gameField.doDraw(mainWindow);

				if (nextBlock)
					nextBlock->doDrawAt(mainWindow, fieldWidth + 1, 0);

				mainWindow.draw(scoreText);
			}
			mainWindow.draw(pauseText);
		}
		else if (gameManager == GameState::LOSE)
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
		if (gameManager == GameState::PLAY)
		{
			if (playerBlock)
			{
				if (playerBlock->doLogic(gameField))
				{
					if (gameField.isFinal())
					{
						gameManager = GameState::LOSE;
					}
					else
					{
						playerBlock.swap(nextBlock);
						nextBlock.reset(getPrimitive(dre, static_cast<int>((fieldWidth - 2) / 2.0), 0));
					}
				}
				if (int cnt = gameField.erasing())
				{
					score += (cnt * 15);
					updateScore();
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Game::doWork()
	{
		Clock gameTime;

		while (mainWindow.isOpen())
		{
			if (gameTime.getElapsedTime().asSeconds() > 1 || 
				(gameManager == GameState::PLAY && gameTime.getElapsedTime().asMilliseconds() > 50 && Keyboard::isKeyPressed(Keyboard::Down)))
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