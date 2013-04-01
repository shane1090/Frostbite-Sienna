#include "stdafx.h"
#include "EditorScreen.h"

EditorScreen::EditorScreen(void)
{
	curLayer = 1;
	mouseDragSegment = -1;
	scroll = sf::Vector2<float>(-640.0f, -360.0f);
}

EditorScreen::~EditorScreen(void)
{

}

void EditorScreen::LoadContent()
{
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;

	text.setString("Editor Screen");
	text.setFont(font);

	LoadSegmentDefinitions();

	drawingMode = SEGMENT_SELECTION;
}

void EditorScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void EditorScreen::Update(sf::RenderWindow &Window, sf::Event event)
{
	input.Update(Window, event);
	
	float scale = 1.0f;

	input.mousePos = sf::Mouse::getPosition(Window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		leftMouseDown = true;
	else
		leftMouseDown = false;

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:

		mouseHoverSegment = GetHoveredSegement(input.mousePos, curLayer);
		
		if (mouseDragSegment > -1)
		{
			if (!leftMouseDown)
				mouseDragSegment = -1;
			else
			{
				sf::Vector2<float> loc = mapSeg[mouseDragSegment]->location;
				loc.x += (input.mousePos.x - input.prevMousePos.x) / scale;
				loc.y += (input.mousePos.y - input.prevMousePos.y) / scale;
				mapSeg[mouseDragSegment]->location = loc;
			}
		}

		if (leftMouseDown && input.mousePos.x < 900 && mouseDragSegment == -1)
			if (mouseHoverSegment != -1)
				mouseDragSegment = mouseHoverSegment;

		if (leftMouseDown && prevLeftMouseDown == false)
		{
			if (mouseHoverSegment != -1)
				mouseSelectedSegment = mouseHoverSegment;
		}

		if (mouseSelectedSegment != -1)
		{
			// Allow deletion of segments
			if (input.KeyPressed(sf::Keyboard::Delete))
			{
				mapSeg.erase(mapSeg.begin()+mouseSelectedSegment);
				mouseSelectedSegment = -1;
				mouseHoverSegment = -1;
			}

			// Allow rotation of segments
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				float angle = atan2((input.mousePos.y - mapSeg[mouseSelectedSegment]->location.y) + scroll.y,
								    (input.mousePos.x - mapSeg[mouseSelectedSegment]->location.x) + scroll.x);

				mapSeg[mouseSelectedSegment]->rotation = angle * (180.0f/M_PI);
			}
		}

		break;
	case LEDGES:

		break;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		scroll.x -= (input.mousePos.x - input.prevMousePos.x) * 2.0f;
		scroll.y -= (input.mousePos.y - input.prevMousePos.y) * 2.0f;
	}

	input.prevMousePos = input.mousePos;
	prevLeftMouseDown = leftMouseDown;
}

void EditorScreen::Draw(sf::RenderWindow &Window)
{
	DrawMap(Window);

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		DrawSegmentSelection(Window);

		if (mouseHoverSegment > -1)
			DrawSelectedSegment(Window, mouseHoverSegment, sf::Color::White);

		if (mouseSelectedSegment > -1)
			DrawSelectedSegment(Window, mouseSelectedSegment, sf::Color::Red);


		break;
	case LEDGES:

		break;
	}

	Window.draw(text);
}

void EditorScreen::DrawMap(sf::RenderWindow &Window)
{
	float scale = 1.0f;

	for (int l = 0; l < 3; l++)
	{
		for (int i = 0; i < mapSeg.size(); i++)
		{
			if (mapSeg[i]->layer == l)
			{
				sf::Rect<float> dRect;
				dRect.left = (mapSeg[i]->location.x - scroll.x) * scale;
				dRect.top = (mapSeg[i]->location.y - scroll.y) * scale;
				dRect.width = (float)segDef[mapSeg[i]->segmentIndex]->width;
				dRect.height = (float)segDef[mapSeg[i]->segmentIndex]->height;

				sf::Sprite segSprite;
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(dRect.left, dRect.top);
				segSprite.setScale(scale, scale);
				segSprite.setOrigin(segDef[mapSeg[i]->segmentIndex]->width / 2, segDef[mapSeg[i]->segmentIndex]->height / 2);
				segSprite.setRotation(mapSeg[i]->rotation);
				Window.draw(segSprite);
			}
		}
	}
}

void EditorScreen::DrawSegmentSelection(sf::RenderWindow &Window)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(975, 20);
	sf::Vector2<float> segmentSize(285, 680);
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(255,255,255,200));
	Window.draw(segmentShape);

	sf::Rect<int> dRect;

	int segCols = 250 / 45;
	int curCol = 0;
	int curRow = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		dRect.left = (985) + (55 * curCol);
		dRect.top = 30 + (curRow * 60);
		dRect.width = 45;
		dRect.height = 45;

		sf::Vector2u texSize = segDef[i]->tex.getSize();

		// Get sprite reference
		sf::Sprite segSprite;
		segSprite.setTexture(segDef[i]->tex);
		segSprite.scale((float)dRect.width / texSize.x, (float)dRect.height / texSize.y);
		segSprite.setPosition((float)dRect.left, (float)dRect.top);
		Window.draw(segSprite);

		// This should be moved to Update
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (input.mousePos.x > dRect.left && input.mousePos.x < (dRect.left + dRect.width) &&
				input.mousePos.y > dRect.top && input.mousePos.y < (dRect.top + dRect.height))
			{
				AddSegment(1, i);
			}
		}

		curCol++;
		if (curCol == segCols) {
			curCol = 0;
			curRow++;
		}
	}
}

void EditorScreen::DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color)
{
	sf::Rect<float> dRect;

	dRect.left = (mapSeg[segment]->location.x - scroll.x) * 1.0f;
	dRect.top = (mapSeg[segment]->location.y - scroll.y) * 1.0f;
	dRect.width = (float)segDef[mapSeg[segment]->segmentIndex]->width * 1.0f;
	dRect.height = (float)segDef[mapSeg[segment]->segmentIndex]->height * 1.0f;


	sf::RectangleShape segmentShape;
	segmentShape.setPosition(dRect.left, dRect.top);
	segmentShape.setRotation(mapSeg[segment]->rotation);
	segmentShape.setOrigin(dRect.width / 2, dRect.height / 2);
	
	sf::Vector2<float> segmentSize(dRect.width, dRect.height);
	
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color::Transparent);
	segmentShape.setOutlineColor(color);
	segmentShape.setOutlineThickness(1);
	Window.draw(segmentShape);
}

void EditorScreen::LoadSegmentDefinitions()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile( "Tilesets/tileset01.xml" );

	tinyxml2::XMLElement* root = doc.FirstChildElement( "tileset" );
	for (tinyxml2::XMLElement* e = root->FirstChildElement( "tile" ); e; e = e->NextSiblingElement() )
	{
		// Todo add validation routines to stop buggy XML files being imported

		int tileId = std::atoi(e->Attribute("id"));
		std::string name = e->Attribute("name");
		std::string filepath = e->Attribute("image");

		segDef.push_back(new SegmentDefinition(name, filepath));
		
	}
}

void EditorScreen::AddSegment(int layer, int index)
{
	std::cout << "Segment Added to Map: " << index << std::endl; 
	mapSeg.push_back(new MapSegment(layer, index));
}

int EditorScreen::GetHoveredSegement(sf::Vector2<int> mousePos, int layer)
{
	float scale = 1.0f;

	int hoveredSegment = -1;

	for (int i = 0; i < mapSeg.size(); i++)
	{
		if (mapSeg[i]->layer == layer)
		{
			sf::Rect<float> dRect(
				((mapSeg[i]->location.x - scroll.x) * scale),
				((mapSeg[i]->location.y - scroll.y) * scale),
				(segDef[mapSeg[i]->segmentIndex]->width * scale),
				(segDef[mapSeg[i]->segmentIndex]->height * scale));

			float c = cos(-mapSeg[i]->rotation * M_PI / 180);
			float s = sin(-mapSeg[i]->rotation * M_PI / 180);

			float rotatedX = dRect.left + c * (mousePos.x - dRect.left) - s * (mousePos.y - dRect.top);
			float rotatedY = dRect.top + s * (mousePos.x - dRect.left) + c * (mousePos.y - dRect.top);

			float leftX = dRect.left - dRect.width / 2;
			float rightX = dRect.left + dRect.width / 2;
			float topY = dRect.top - dRect.height / 2;
			float bottomY = dRect.top + dRect.height / 2;

			if (leftX <= rotatedX && rotatedX <= rightX &&
				topY <= rotatedY && rotatedY <= bottomY)
			{
				hoveredSegment = i;
			}
		}
	}

	return hoveredSegment;
}
