#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "GameScreen.h"
#include "ScreenManager.h"
#include "SegmentDefinition.h"
#include "MapSegment.h"

class EditorScreen  : public GameScreen
{
public:
	EditorScreen(void);
	~EditorScreen(void);

	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Event event);
	void Draw(sf::RenderWindow &window);
	void DrawSegmentSelection(sf::RenderWindow &Window);
	void DrawMap(sf::RenderWindow &Window);
	void DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color);
	void AddSegment(int layer, int index);

protected:
private:
	void LoadSegmentDefinitions();
	int GetHoveredSegement(sf::Vector2<int> mousePos, int layer);

	sf::Text text;
	sf::Font font;

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;

	enum drawingMode_t { SEGMENT_SELECTION, LEDGES };
	drawingMode_t drawingMode;

	sf::Vector2<float> scroll;

	int curLayer;
	int mouseDragSegment, mouseHoverSegment, mouseSelectedSegment;

	bool leftMouseDown, prevLeftMouseDown;

};

#endif // EDITORSCREEN_H