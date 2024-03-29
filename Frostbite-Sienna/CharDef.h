#pragma once

#include "Animation.h"
#include "Frame.h"

class CharDef
{
public:
	CharDef(void);
	~CharDef(void);

	void LoadCharacter(std::string filePath);
	void SaveCharacter(std::string filePath);

	std::vector<Animation*> animations;
	std::vector<Frame*> frames;
	sf::String path;

	int headIndex;
	int torsoIndex;
	int legsIndex;
	int weaponIndex;
};

