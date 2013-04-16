#pragma once

#include "GameScreen.h"
#include "ScreenManager.h"
#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "Ledge.h"
#include "Character.h"
#include "Map.h"

class PlayTestScreen : public GameScreen
{
public:
	PlayTestScreen(void);
	~PlayTestScreen(void);

	void LoadContent();
	void UnloadContent();
	void SetMapData(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &window, sf::Clock &gameTime);
	void DrawMap(sf::RenderWindow &Window);

private:
	Map *map;
	Character *character;

	sf::Vector2<float> scroll;

	sf::Font font;

	char fpsBuffer[32];
	sf::Text fpsCounter;
};

