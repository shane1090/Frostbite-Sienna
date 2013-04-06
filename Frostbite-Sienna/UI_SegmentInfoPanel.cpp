#include "stdafx.h"
#include "UI_SegmentInfoPanel.h"

SegmentInfoPanel::SegmentInfoPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg) : segDef(segDef), mapSeg(mapSeg)
{
	panelPos = sf::Vector2<int>(985, 225);
	segmentSize = sf::Vector2<float>(295, 200);
}


SegmentInfoPanel::~SegmentInfoPanel(void)
{

}

void SegmentInfoPanel::LoadContent(sf::Font &font)
{
	this->font = font;
}

void SegmentInfoPanel::Update()
{

}

void SegmentInfoPanel::Draw(sf::RenderWindow &Window, int curSelectedSegment)
{
	sf::RectangleShape panelShape;
	panelShape.setPosition(panelPos.x, panelPos.y);
	sf::Vector2<float> panelSize(segmentSize.x, segmentSize.y);
	panelShape.setSize(panelSize);
	panelShape.setFillColor(sf::Color(0,0,0,200));
	Window.draw(panelShape);

	text.setString("Current Segment: " + segDef[curSelectedSegment]->name);
	text.setPosition(panelPos.x + 10, segmentSize.y + 30);
	text.setFont(font);
	text.setColor(sf::Color(255,255,255,255));
	text.setCharacterSize(14);
	Window.draw(text);

	text.setString("Position: " + Convert(mapSeg[curSelectedSegment]->position.x) + "," + Convert(mapSeg[curSelectedSegment]->position.y) + "");
	text.setPosition(panelPos.x + 10, segmentSize.y + 55);
	text.setFont(font);
	text.setColor(sf::Color(255,255,255,255));
	text.setCharacterSize(14);
	Window.draw(text);

	text.setString("Scale: " + Convert(mapSeg[curSelectedSegment]->scale.x) + "," + Convert(mapSeg[curSelectedSegment]->scale.y) + "");
	text.setPosition(panelPos.x + 10, segmentSize.y + 80);
	text.setFont(font);
	text.setColor(sf::Color(255,255,255,255));
	text.setCharacterSize(14);
	Window.draw(text);

	text.setString("Rotation: " + Convert(mapSeg[curSelectedSegment]->rotation));
	text.setPosition(panelPos.x + 10, segmentSize.y + 105);
	text.setFont(font);
	text.setColor(sf::Color(255,255,255,255));
	text.setCharacterSize(14);
	Window.draw(text);

	text.setString("Physics Object: " + (sf::String)((mapSeg[curSelectedSegment]->physicsObject) ? "Yes" : "No"));
	text.setPosition(panelPos.x + 10, segmentSize.y + 130);
	text.setFont(font);
	text.setColor(sf::Color(255,255,255,255));
	text.setCharacterSize(14);
	Window.draw(text);

	if (mapSeg[curSelectedSegment]->physicsObject)
	{
		text.setString("Physics Weight: " + mapSeg[curSelectedSegment]->physicsWeight);
		text.setPosition(panelPos.x + 10, segmentSize.y + 155);
		text.setFont(font);
		text.setColor(sf::Color(255,255,255,255));
		text.setCharacterSize(14);
		Window.draw(text);
	}
}
