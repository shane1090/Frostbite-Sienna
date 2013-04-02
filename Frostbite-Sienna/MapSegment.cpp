#include "stdafx.h"
#include "MapSegment.h"

MapSegment::MapSegment(int _layer, int _index, sf::Vector2<float> position)
{
	this->position = position;
	segmentIndex = _index;
	layer = _layer;
	physicsObject = false;
	physicsWeight = 0;
	rotation = 0;
	scaleX = 0;
	scaleY = 0;
}