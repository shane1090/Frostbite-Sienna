#pragma once

class KeyFrame
{
public:
	KeyFrame(int frameRef, int duration);
	~KeyFrame(void);

	int frameRef;
	int duration;
	std::vector<sf::String> scripts;
};

