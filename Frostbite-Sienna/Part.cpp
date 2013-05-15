#include "stdafx.h"
#include "Part.h"

Part::Part(int index)
{
	this->index = index;
	scaling = sf::Vector2f(1.0f, 1.0f);
	rotation = 0.0f;
}


Part::~Part(void)
{

}
