#pragma once

#include "GameScreen.h"
#include "ScreenManager.h"
#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "SegmentPanel.h"
#include "shobjidl.h"

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
	void DrawToolBar(sf::RenderWindow &Window);
	bool DrawButton(sf::RenderWindow &Window, int x, int y, int index);
	std::string Convert(float number);
	void SaveMap();
	void LoadMap();

private:
	void LoadSegmentDefinitions();
	void ResetMap();
	std::string utf8_encode(const std::wstring &wstr);

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

	// Text elements
	sf::Text scrollPosText, curLayerText, curZoomLevelText;

	sf::Texture toolbarIconsTex;

	std::string curFile;

	float zoomScale;
};

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"Map Editor XML (*.xml)",       L"*.xml"}
};