#pragma once

class Layer
{
public:
	Layer(std::string name, float scale);
	~Layer(void);

	std::string layerName;
	float scale;
};

