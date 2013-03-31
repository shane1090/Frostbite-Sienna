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

protected:
private:
	void LoadSegmentDefinitions();

	sf::Text text;
	sf::Font font;

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;

	enum drawingMode_t { SEGMENT_SELECTION, LEDGES };
	drawingMode_t drawingMode;

};

#endif // EDITORSCREEN_H