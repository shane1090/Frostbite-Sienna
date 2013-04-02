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
	void Draw(InputManager &input, sf::RenderWindow &Window);
	void Update(int curLayer, InputManager &input, sf::Vector2<float> scroll);
	void AddSegment(int layer, int index, sf::Vector2<float> scroll);

protected:
private:
	std::vector<SegmentDefinition*> &segDef;
	std::vector<MapSegment*> &mapSeg;

	int scrollRow;
	int offset;
	int maxSegments;

};

#endif // SEGMENTPANEL_H

