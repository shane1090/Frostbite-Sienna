#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "GameScreen.h"
#include "ScreenManager.h"
#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "SegmentPanel.h"

class EditorScreen  : public GameScreen
{
public:
	EditorScreen(void);
	~EditorScreen(void);

	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Event event);
	void Draw(sf::RenderWindow &window);
	void DrawMap(sf::RenderWindow &Window);
	void DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color);
	void AddSegment(int layer, int index);

private:
	void LoadSegmentDefinitions();
	int GetHoveredSegement(sf::Vector2<int> mousePos, int layer);

	sf::Font font;

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;
	std::vector<float> layerScales;

	enum drawingMode_t { SEGMENT_SELECTION, LEDGES };
	drawingMode_t drawingMode;

	sf::Vector2<float> scroll;

	int curLayer;
	int mouseDragSegment, mouseHoverSegment, mouseSelectedSegment;

	bool leftMouseDown, prevLeftMouseDown;

	SegmentPanel* segmentPanel;

};

#endif // EDITORSCREEN_H