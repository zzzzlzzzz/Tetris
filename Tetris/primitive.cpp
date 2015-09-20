#include "primitive.h"

using namespace std;

using namespace sf;

namespace GameSpace
{
	//////////////////////////////////////////////////////////////////////////
	Field::Field(int width, int height)
		:field(height, vector<Primitive::BlockColor>(width, Primitive::BlockColor::EMPTY))
	{
		for (int y = 0; y < height; y++)
		{
			field[y][0] = Primitive::BlockColor::YELLOW;
			field[y][width - 1] = Primitive::BlockColor::YELLOW;
		}
		for (int x = 1; x < width - 1; x++)
			field[height - 1][x] = Primitive::BlockColor::YELLOW;
	}
	//////////////////////////////////////////////////////////////////////////
	void Field::doDraw(sf::RenderWindow& render)
	{
		for (size_t y = 0; y < field.size(); y++)
		{
			for (size_t x = 0; x < field[y].size(); x++)
			{
				if (field[y][x] != Primitive::BlockColor::EMPTY)
					Primitive::drawBlock(render, x, y, field[y][x]);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Field::isEmpty(int x, int y)
	{
		if (y >= 0 && x >= 0 && y < static_cast<int>(field.size()) && x < static_cast<int>(field[y].size()))
			return field[y][x] == Primitive::BlockColor::EMPTY;
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void Field::setColor(int x, int y, Primitive::BlockColor color)
	{
		if (y >= 0 && x >= 0 && y < static_cast<int>(field.size()) && x < static_cast<int>(field[y].size()))
			field[y][x] = color;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Field::isFinal()
	{
		// if last 2 line not empty - player lose
		for (size_t y = 0; y < 3; y++)
		{
			for (size_t x = 1; x < field[y].size() - 1; x++)
			{
				if (field[y][x] != Primitive::BlockColor::EMPTY)
					return true;
			}
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void Field::erasing()
	{
		for (size_t y = 0; y < field.size() - 1; y++)
		{
			bool haveLine = true;
			for (size_t x = 1; x < field[y].size() - 1 && haveLine; x++)
			{
				if (field[y][x] == Primitive::BlockColor::EMPTY)
					haveLine = false;
			}
			if (haveLine)
			{
				for (int iy = static_cast<int>(y); iy > 0; iy--)
				{
					for (size_t x = 1; x < field[y].size() - 1; x++)
					{
						field[iy][x] = field[iy - 1][x];
						field[iy - 1][x] = Primitive::BlockColor::EMPTY;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Primitive::Primitive(const std::vector<std::vector<bool>>& tpl, std::default_random_engine& dre, int positionX, int positionY)
		:posX(positionX), posY(positionY), state(0)
	{
		BlockColor schemeColor;
		switch (uniform_int_distribution<int>(0,2)(dre))
		{
		case 0:
			schemeColor = BlockColor::RED;
			break;
		case 1:
			schemeColor = BlockColor::GREEN;
			break;
		case 2:
			schemeColor = BlockColor::BLUE;
			break;
		default:
			schemeColor = BlockColor::EMPTY;
			break;
		}

		for (auto& line : tpl)
		{
			vector<BlockColor> coloredLine;
			for (auto item : line)
			{
				coloredLine.push_back(item ? schemeColor : BlockColor::EMPTY);
			}
			scheme.push_back(coloredLine);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::drawBlock(sf::RenderWindow& render, int positionX, int positionY, BlockColor color)
	{
		RectangleShape rshape(Vector2f(static_cast<float>(blockSize), static_cast<float>(blockSize)));
		rshape.setPosition(Vector2f(static_cast<float>(positionX * blockSize), static_cast<float>(positionY * blockSize)));
		switch (color)
		{
		case BlockColor::BLUE:
			rshape.setFillColor(Color::Blue);
			rshape.setOutlineColor(Color::Blue);
			render.draw(rshape);
			break;
		case BlockColor::GREEN:
			rshape.setFillColor(Color::Green);
			rshape.setOutlineColor(Color::Green);
			render.draw(rshape);
			break;
		case BlockColor::RED:
			rshape.setFillColor(Color::Red);
			rshape.setOutlineColor(Color::Red);
			render.draw(rshape);
			break;
		case BlockColor::YELLOW:
			rshape.setFillColor(Color::Yellow);
			rshape.setOutlineColor(Color::Yellow);
			render.draw(rshape);
			break;
		default:
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::doDraw(sf::RenderWindow& render)
	{
		for (size_t y = 0; y < scheme.size(); y++)
		{
			for (size_t x = 0; x < scheme[y].size(); x++)
			{
				if (scheme[y][x] != BlockColor::EMPTY)
					drawBlock(render, posX + x, posY + y, scheme[y][x]);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Primitive::haveDownBorder(Field& field)
	{
		bool stop = false;
		for (size_t y = 0; y < scheme.size() && !stop; y++)
		{
			for (size_t x = 0; x < scheme[y].size() && !stop; x++)
			{
				if (scheme[y][x] != BlockColor::EMPTY && !field.isEmpty(posX + x, posY + y + 1))
					stop = true;
			}
		}
		return stop;
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::normalize()
	{
		// прижимаем к левой границе
		while (1)
		{
			bool leftEmpty = true;
			for (size_t y = 0; y < scheme.size() && leftEmpty; y++)
			{
				if (scheme[y][0] != BlockColor::EMPTY)
					leftEmpty = false;
			}
			if (leftEmpty)
			{
				for (size_t y = 0; y < scheme.size(); y++)
				{
					for (size_t x = 0; x < scheme[y].size() - 1; x++)
					{
						scheme[y][x] = scheme[y][x + 1];
						scheme[y][x + 1] = BlockColor::EMPTY;
					}
				}
			}
			else
				break;
		}
		// прижимаем к верхней границе
		while (1)
		{
			bool topEmpty = true;
			for (size_t x = 0; x < scheme[0].size() && topEmpty; x++)
			{
				if (scheme[0][x] != BlockColor::EMPTY)
					topEmpty = false;
			}

			if (topEmpty)
			{
				for (size_t y = 0; y < scheme.size() - 1; y++)
				{
					for (size_t x = 0; x < scheme[y].size(); x++)
					{
						scheme[y][x] = scheme[y + 1][x];
						scheme[y + 1][x] = BlockColor::EMPTY;
					}
				}
			}
			else
				break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Primitive::doLogic(Field& field)
	{
		bool stop = haveDownBorder(field);

		if (!stop)
			posY += 1;
		else
		{
			for (size_t y = 0; y < scheme.size(); y++)
			{
				for (size_t x = 0; x < scheme[y].size(); x++)
				{
					if (scheme[y][x] != BlockColor::EMPTY)
						field.setColor(posX + x, posY + y, scheme[y][x]);
				}
			}
		}

		return stop;
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::rotLeft(Field& field)
	{
		vector<vector<BlockColor>> buffer = scheme;

		if (state == 0 || state == 2)
		{
			for (size_t y = 0; y < buffer.size(); y++)
			{
				for (size_t x = 0; x < buffer[y].size(); x++)
				{
					scheme[y][x] = buffer[(buffer.size() - 1) - x][(buffer[y].size() - 1) - y];
				}
			}
			if (--state < 0)
				state = 3;
		}
		else if (state == 1 || state == 3)
		{
			for (size_t y = 0; y < buffer.size(); y++)
			{
				for (size_t x = 0; x < buffer[y].size(); x++)
				{
					scheme[y][x] = buffer[x][y];
				}
			}
			--state;
		}

		if (haveDownBorder(field))
			scheme = buffer;
		else
			normalize();
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::rotRight(Field& field)
	{
		vector<vector<BlockColor>> buffer = scheme;

		if (state == 0 || state == 2)
		{
			for (size_t y = 0; y < buffer.size(); y++)
			{
				for (size_t x = 0; x < buffer[y].size(); x++)
				{
					scheme[y][x] = buffer[x][y];
				}
			}
			++state;
		}
		else if (state == 1 || state == 3)
		{
			for (size_t y = 0; y < buffer.size(); y++)
			{
				for (size_t x = 0; x < buffer[y].size(); x++)
				{
					scheme[y][x] = buffer[(buffer.size() - 1) - x][(buffer[y].size() - 1) - y];
				}
			}
			
			if (++state > 3)
				state = 0;
		}

		if (haveDownBorder(field))
			scheme = buffer;
		else
			normalize();
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::moveRight(Field& field)
	{
		bool moveAllow = true;
		for (size_t y = 0; y < scheme.size() && moveAllow; y++)
		{
			for (int x = static_cast<int>(scheme[y].size()) - 1; x >= 0 && moveAllow; x--)
			{
				if (scheme[y][x] != BlockColor::EMPTY && !field.isEmpty(posX + x + 1, posY + y))
					moveAllow = false;
			}
		}
		if (moveAllow)
			++posX;
	}
	//////////////////////////////////////////////////////////////////////////
	void Primitive::moveLeft(Field& field)
	{
		bool moveAllow = true;
		for (size_t y = 0; y < scheme.size() && moveAllow; y++)
		{
			for (size_t x = 0; x < scheme[y].size() && moveAllow; x++)
			{
				if (scheme[y][x] != BlockColor::EMPTY && !field.isEmpty(posX + x - 1, posY + y))
					moveAllow = false;
			}
		}
		if (moveAllow)
			--posX;
	}
	//////////////////////////////////////////////////////////////////////////
	Point::Point(std::default_random_engine& dre, int positionX, int positionY) 
		:Primitive({ { true, false, false },
					 { false, false, false},
					 { false, false, false} }, 
					 dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Pe::Pe(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive({	{ true, true, true },
						{ true, false, true },
						{ true, false, true } },
					dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Triangle::Triangle(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive({	{ true, true, true },
						{ false, true, false },
						{ false, false, false } },
					dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Te::Te(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive({	{ true, true, true },
						{ false, true, false },
						{ false, true, false } },
					dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Block::Block(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive(	{	{ true, true, false },
						{ true, true, false },
						{ false, false, false } },
						dre, positionX, positionY)
	{
	
	}

	//////////////////////////////////////////////////////////////////////////

	ZigZag::ZigZag(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive({	{ false, true, true },
						{ true, true, false },
						{ false, false, false } },
						dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Line::Line(std::default_random_engine& dre, int positionX, int positionY)
		:Primitive({	{ true, true, true },
						{ false, false, false },
						{ false, false, false } },
						dre, positionX, positionY)
	{

	}

	//////////////////////////////////////////////////////////////////////////

	Primitive* getPrimitive(std::default_random_engine& dre, int positionX, int positionY)
	{
		switch (uniform_int_distribution<int>(0, 6)(dre))
		{
		case 0:
			return new Point(dre, positionX, positionY);
		case 1:
			return new Pe(dre, positionX, positionY);
		case 2:
			return new Triangle(dre, positionX, positionY);
		case 3:
			return new Te(dre, positionX, positionY);
		case 4:
			return new Block(dre, positionX, positionY);
		case 5:
			return new ZigZag(dre, positionX, positionY);
		case 6:
			return new Line(dre, positionX, positionY);
		default:
			return nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
}