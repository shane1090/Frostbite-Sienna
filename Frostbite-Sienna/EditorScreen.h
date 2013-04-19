#pragma once

#include "GameScreen.h"
#include "ScreenManager.h"
#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "Ledge.h"
#include "UI_SegmentInfoPanel.h"
#include "UI_LedgePanel.h"
#include "shobjidl.h"
#include "InputManager.h"
#include "PanelManager.h"
#include "Map.h"
#include "UI_SegmentPanel.h"

class EditorScreen : public GameScreen
{
public:
	EditorScreen(void);
	~EditorScreen(void);

	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &window, sf::Clock &gameTime);
	void DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color);
	void DrawToolBar(sf::RenderWindow &Window);
	bool DrawButton(sf::RenderWindow &Window, int x, int y, int index);
	void SaveMap();
	void LoadMap();
	void SetMapData(Map *map);

private:
	Map *map;
	
	void ResetMap();

	int GetHoveredSegement(sf::Vector2<int> mousePos, int layer);
	int GetHoveredLedgeNode(sf::Vector2<int> mousePos);

	sf::Font font;

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;
	std::vector<Ledge*> ledges;
	std::vector<float> layerScales;

	enum drawingMode_t { SEGMENT_SELECTION, LEDGES, DMODE_LAST };
	drawingMode_t drawingMode;

	sf::Vector2<float> scroll;

	int tile;

	int curLayer;
	int mouseDragSegment, mouseHoverSegment, mouseSelectedSegment;
	int mouseDragNode, mouseHoverNode, mouseSelectedNode;

	int curLedge;
	int curNode;

	bool scrollMap;

	SegmentInfoPanel* segmentInfoPanel;
	LedgePanel* ledgePanel;

	// Text elements
	sf::Text scrollPosText, curLayerText, curZoomLevelText, curDrawingMode;

	sf::Texture toolbarIconsTex;

	std::string curFile;

	float zoomScale;

	sf::Vector2<int> mousePos, pMousePos;

	PanelManager *panelManager;
	Panel *segmentPane;
};

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"Map Editor XML (*.xml)",       L"*.xml"}
};