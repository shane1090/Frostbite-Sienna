#pragma once

#include "Part.h"

class Frame
{
public:
	Frame(void);
	~Frame(void);

	std::vector<Part*> parts;
	sf::String name;
};

