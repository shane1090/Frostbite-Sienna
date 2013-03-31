#pragma once

class SegmentDefinition
{
public:
	SegmentDefinition(std::string _name, std::string _tex);

	std::string name;
	sf::Texture tex;
	int width;
	int height;

private:

};