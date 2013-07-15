#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharPreview : public Panel
{
public:
	UICharPreview(CharDef *&charDef, std::vector<sf::Texture> &legsTex, std::vector<sf::Texture> &torsoTex, std::vector<sf::Texture> &headTex, int &selAnim, int &selKeyFrame);
	~UICharPreview(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	void DrawCharacter(sf::Vector2f loc, float scale, int face, int frameIndex, bool preview, float alpha, sf::RenderWindow &Window);

private:
	CharDef *charDef;
	sf::Text text;

	int FACE_LEFT;
	int FACE_RIGHT;

	int &selAnim;
	int &selKeyFrame;
	int curKey;

	bool playing;

	float curFrame;

	std::vector<sf::Texture> &legsTex;
	std::vector<sf::Texture> &torsoTex;
	std::vector<sf::Texture> &headTex;
};

