#pragma once

class Ledge
{
public:
	Ledge(void);
	~Ledge(void);

	std::vector<sf::Vector2<float>> nodes;
	int totalNodes;
	int flags;
};

