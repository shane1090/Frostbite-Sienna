#pragma once

class MapSegment
{
public:
	MapSegment(int _layer, int _index, sf::Vector2<float> position, float rotation, sf::Vector2<float> scale);
	sf::Vector2<float> position;
	sf::Vector2<float> scale;
	int segmentIndex;
	bool physicsObject;
	int physicsWeight;
	float rotation;
	int layer;

private:

};