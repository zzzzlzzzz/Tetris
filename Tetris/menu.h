#ifndef MENU_H
#define MENU_H

#include <string>
#include <stdexcept>

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
		Menu(const std::string& fontPath);
		void doDraw(sf::RenderWindow& render);
		bool doEvent(const sf::Event& evItem, sf::RenderWindow& render);
		FieldSize getSize(){ return selSize; }
	};
}

#endif // !MENU_H
