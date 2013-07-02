#pragma once

#include "KeyFrame.h"

class Animation
{
public:
	Animation(void);
	~Animation(void);

	std::vector<KeyFrame*> keyFrames;
	std::string name;
};

