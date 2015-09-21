#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

namespace GameSpace
{
	class GameManager
	{
	public:
		enum class GameState{ MENU, PAUSE, PLAY, LOSE };
	private:
		GameState gameState;
	public:
		GameManager() :gameState(GameState::MENU){}
		GameState getState(){ return gameState; }
		void setState(GameState state){ gameState = state; }
		bool isPlay(){ return gameState == GameState::PLAY; }
	};
}

#endif // !GAMEMANAGER_H
