#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

namespace GameSpace
{
	class Menu
	{
	public:
		enum class FieldSize{ MINIMAL, STANDART, MAXIMAL };
	private:
		sf::Font menuFont;
		sf::Text menuItemSizeMin;
		sf::Text menuItemSizeStd;
		sf::Text menuItemSizeMax;
		sf::Text menuItemStartGame;
		sf::Text menuItemExitGame;
		sf::Text description;
		FieldSize selSize;
	public:
		Menu();
		void doDraw(sf::RenderWindow& render);
		bool doEvent(const sf::Event& evItem, sf::RenderWindow& render);
		FieldSize getSize(){ return selSize; }
	};
}

#endif // !MENU_H
