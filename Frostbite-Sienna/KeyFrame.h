#pragma once

class KeyFrame
{
public:
	KeyFrame(void);
	~KeyFrame(void);

	int frameRef;
	int duration;
	std::vector<sf::String> scripts;
};

