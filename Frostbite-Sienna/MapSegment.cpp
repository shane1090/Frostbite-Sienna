#include "stdafx.h"
#include "MapSegment.h"

MapSegment::MapSegment(int _layer, int _index, sf::Vector2<float> position, float rotation, sf::Vector2<float> scale)
{
	this->position = position;
	segmentIndex = _index;
	layer = _layer;
	physicsObject = false;
	physicsWeight = 0;
	this->rotation = rotation;
	this->scale = scale;
}