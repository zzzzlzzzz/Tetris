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
		enum class BlockColor{ EMPTY, RED, GREEN, BLUE, YELLOW, 
								CFFFF66, CFFCC33, CFF6600, CFF0066, CFF0099, 
								CCC00CC, C6600CC, C3300CC, C00CCFF, C33FFCC, 
								C33FF66, C33FF00, CCCFF33, C999999 };
		static const int blockSize = 20;
		static void drawBlock(sf::RenderWindow& render, int positionX, int positionY, BlockColor color);
		//////////////////////////////////////////////////////////////////////////
		Primitive(const std::vector<std::vector<bool>>& tpl, std::default_random_engine& dre, int positionX, int positionY);
		void doDraw(sf::RenderWindow& render);
		bool doLogic(Field& field);
		void rotRight(Field& field);
		void rotLeft(Field& field);
		void moveRight(Field& field);
		void moveLeft(Field& field);
		void doDrawAt(sf::RenderWindow& render, int positionX, int positionY);
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
		int erasing();
	};

	class Point:public Primitive
	{
	public:
		Point(std::default_random_engine& dre, int positionX, int positionY);
	};

	class Pe :public Primitive
	{
	public:
		Pe(std::default_random_engine& dre, int positionX, int positionY);
	};

	class Triangle :public Primitive
	{
	public:
		Triangle(std::default_random_engine& dre, int positionX, int positionY);
	};

	class Te :public Primitive
	{
	public:
		Te(std::default_random_engine& dre, int positionX, int positionY);
	};

	class Block :public Primitive
	{
	public:
		Block(std::default_random_engine& dre, int positionX, int positionY);
	};

	class ZigZag :public Primitive
	{
	public:
		ZigZag(std::default_random_engine& dre, int positionX, int positionY);
	};

	class ZigZag2 :public Primitive
	{
	public:
		ZigZag2(std::default_random_engine& dre, int positionX, int positionY);
	};

	class Line :public Primitive
	{
	public:
		Line(std::default_random_engine& dre, int positionX, int positionY);
	};

	Primitive* getPrimitive(std::default_random_engine& dre, int positionX, int positionY);
}

#endif // !PRIMITIVE_H
