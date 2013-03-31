#include "stdafx.h"
#include "SegmentDefinition.h"

SegmentDefinition::SegmentDefinition(std::string _name, std::string _tex)
{
	name = _name;
	tex.loadFromFile(_tex);

	sf::Vector2u texSize = tex.getSize();

	width = texSize.x;
	height = texSize.y;
}