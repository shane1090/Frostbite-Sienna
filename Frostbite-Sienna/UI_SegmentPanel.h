#ifndef SEGMENTPANEL_H
#define SEGMENTPANEL_H

#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "InputManager.h"

class SegmentPanel
{
public:
	SegmentPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg);
	~SegmentPanel(void);
	void Draw(int curLayer, sf::Vector2<float> scroll, sf::RenderWindow &Window);
	void Update();
	void AddSegment(int layer, int index, sf::Vector2<float> scroll);

	/*sf::Vector2<int> panelPos;
	sf::Vector2<float> segmentSize;*/

	sf::Rect<float> panelRect;

protected:
private:
	std::vector<SegmentDefinition*> &segDef;
	std::vector<MapSegment*> &mapSeg;

	int scrollRow;
	int offset;

	sf::Vector2<int> mousePos;

};

const int MAX_SEGMENT_COLS = 5;
const int MAX_SEGMENT_ROWS = 5;
const int MAX_SEGMENT_SIZE = 45;
const int SEGMENT_PADDING = 10;
const int MAX_SEGMENTS = MAX_SEGMENT_COLS * MAX_SEGMENT_ROWS;

#endif // SEGMENTPANEL_H

