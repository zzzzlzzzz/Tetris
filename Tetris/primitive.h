#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include <random>

#include <SFML/Graphics.hpp>

namespace GameSpace
{
	class Field;

	class Primitive
	{
		bool haveDownBorder(Field& field);
		void normalize();
	public:
		//////////////////////////////////////////////////////////////////////////
		enum class BlockColor{ EMPTY, RED, GREEN, BLUE, YELLOW };
		static const int blockSize = 20;
		static void drawBlock(sf::RenderWindow& render, int positionX, int positionY, BlockColor color);
		//////////////////////////////////////////////////////////////////////////
		Primitive(const std::vector<std::vector<bool>>& tpl, std::default_random_engine& dre, int positionX, int positionY);
		virtual void doDraw(sf::RenderWindow& render) = 0;
		bool doLogic(Field& field);
		void rotRight(Field& field);
		void rotLeft(Field& field);
		//void moveRight(Field& field);
	protected:
		std::vector<std::vector<BlockColor>> scheme;
		int posX, posY, state;
	};

	class Field
	{
		std::vector<std::vector<Primitive::BlockColor>> field;
	public:
		Field() = default;
		Field(int width, int height);
		void doDraw(sf::RenderWindow& render);
		bool isEmpty(int x, int y);
		void setColor(int x, int y, Primitive::BlockColor color);
		bool isFinal();
	};

	class Point:public Primitive
	{
	public:
		Point(std::default_random_engine& dre, int positionX, int positionY);
		virtual void doDraw(sf::RenderWindow& render) override;
	};

	class Pe :public Primitive
	{
	public:
		Pe(std::default_random_engine& dre, int positionX, int positionY);
		virtual void doDraw(sf::RenderWindow& render) override;
	};

	class Triangle :public Primitive
	{
	public:
		Triangle(std::default_random_engine& dre, int positionX, int positionY);
		virtual void doDraw(sf::RenderWindow& render) override;
	};
}

#endif // !PRIMITIVE_H
