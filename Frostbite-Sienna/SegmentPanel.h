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
	void Update(int curLayer, InputManager &input, std::vector<MapSegment*> &mapSeg);
	void AddSegment(int layer, int index);

protected:
private:
	std::vector<SegmentDefinition*> &segDef;
	std::vector<MapSegment*> &mapSeg;

	int scrollRow;

};

#endif // SEGMENTPANEL_H

