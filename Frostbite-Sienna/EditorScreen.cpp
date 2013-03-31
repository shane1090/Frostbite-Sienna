#include "stdafx.h"
#include "EditorScreen.h"

EditorScreen::EditorScreen(void)
{

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
	
}

void EditorScreen::Draw(sf::RenderWindow &Window)
{
	Window.draw(text);

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		DrawSegmentSelection(Window);
		break;
	case LEDGES:

		break;
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

		curCol++;
		if (curCol == segCols) {
			curCol = 0;
			curRow++;
		}
	}
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
