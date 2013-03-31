#pragma once

class MapSegment
{
public:
	MapSegment(int _layer, int _index);
	sf::Vector2<float> location;
	int segmentIndex;
	bool physicsObject;
	int physicsWeight;
	float rotation;
	float scaleX;
	float scaleY;
	int layer;

private:

};