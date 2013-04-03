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

	if (!toolbarIconsTex.loadFromFile("Assets/GUI/editor-icons.png"))
		std::cout << "Could not load toolbar icons" << std::endl;

	LoadSegmentDefinitions();

	drawingMode = SEGMENT_SELECTION;

	segmentPanel = new SegmentPanel(segDef, mapSeg);

	// Set parallax scrolling scales for each layer
	layerScales.push_back(0.75f);
	layerScales.push_back(1.0f);
	layerScales.push_back(1.25f);
}

void EditorScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void EditorScreen::Update(sf::RenderWindow &Window, sf::Event event)
{
	input.Update(Window, event);

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
				sf::Vector2<float> loc = mapSeg[mouseDragSegment]->position;
				loc.x += (input.mousePos.x - input.prevMousePos.x) / layerScales[curLayer];
				loc.y += (input.mousePos.y - input.prevMousePos.y) / layerScales[curLayer];
				mapSeg[mouseDragSegment]->position = loc;
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
				std::cout << "Map segment deleted: " << mouseSelectedSegment << std::endl;
				mapSeg.erase(mapSeg.begin()+mouseSelectedSegment);
				mouseSelectedSegment = -1;
				mouseHoverSegment = -1;
			}

			// Allow rotation of segments
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				float angle = atan2((input.mousePos.y - mapSeg[mouseSelectedSegment]->position.y) + scroll.y,
								    (input.mousePos.x - mapSeg[mouseSelectedSegment]->position.x) + scroll.x);

				mapSeg[mouseSelectedSegment]->rotation = angle * (180.0f/M_PI);
			}

			// Allow scaling of segments
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				mapSeg[mouseSelectedSegment]->scaleX = ((input.mousePos.x - mapSeg[mouseSelectedSegment]->position.x) + scroll.x);
				mapSeg[mouseSelectedSegment]->scaleY = ((input.mousePos.y - mapSeg[mouseSelectedSegment]->position.y) + scroll.y);

				std::cout << "Scale X: " << mapSeg[mouseSelectedSegment]->scaleX << std::endl;
				std::cout << "Scale Y: " << mapSeg[mouseSelectedSegment]->scaleY << std::endl;
			}
		}

		if (input.KeyPressed(sf::Keyboard::L))
		{
			curLayer = (curLayer + 1) % 3;
			mouseSelectedSegment = -1;
			mouseHoverSegment = -1;
			mouseDragSegment = -1;
			std::cout << "Layer changed to: " << curLayer << std::endl;
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		scroll = sf::Vector2<float>(-640.0f, -360.0f);
	}

	segmentPanel->Update(curLayer, input, scroll);

	input.prevMousePos = input.mousePos;
	prevLeftMouseDown = leftMouseDown;
}

void EditorScreen::Draw(sf::RenderWindow &Window)
{
	DrawMap(Window);

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		if (mouseHoverSegment > -1)
			DrawSelectedSegment(Window, mouseHoverSegment, sf::Color::White);

		if (mouseSelectedSegment > -1)
			DrawSelectedSegment(Window, mouseSelectedSegment, sf::Color::Red);

		segmentPanel->Draw(input, Window);
		break;
	case LEDGES:

		break;
	}

	std::string layerName = "map";
	switch (curLayer)
	{
	case 0:
		layerName = "back";
		break;
	case 1:
		layerName = "mid";
		break;
	case 2:
		layerName = "fore";
		break;
	}

	curLayerText.setString("Current Layer: " + layerName);
	curLayerText.setColor(sf::Color(255,255,255,255));
	curLayerText.setPosition(10, 675);
	curLayerText.setFont(font);
	curLayerText.setCharacterSize(14);
	Window.draw(curLayerText);

	scrollPosText.setString("Position: " + Convert(scroll.x + 640.0f) + "," + Convert(scroll.y + 360.0f));
	scrollPosText.setColor(sf::Color(255,255,255,255));
	scrollPosText.setPosition(10, 695);
	scrollPosText.setFont(font);
	scrollPosText.setCharacterSize(14);
	Window.draw(scrollPosText);

	DrawToolBar(Window);

	if (DrawButton(Window, 65 , 5 , 2))
		SaveMap();

	if (DrawButton(Window, 35 , 5 , 1))
		LoadMap();

	if (DrawButton(Window, 5 , 5 , 0))
		ResetMap();
}

void EditorScreen::DrawMap(sf::RenderWindow &Window)
{
	for (int l = 0; l < 3; l++)
	{
		for (int i = 0; i < mapSeg.size(); i++)
		{
			if (mapSeg[i]->layer == l)
			{
				sf::Rect<float> dRect;
				dRect.left = (mapSeg[i]->position.x - scroll.x) * layerScales[l];
				dRect.top = (mapSeg[i]->position.y - scroll.y) * layerScales[l];
				dRect.width = (float)segDef[mapSeg[i]->segmentIndex]->width;
				dRect.height = (float)segDef[mapSeg[i]->segmentIndex]->height;

				sf::Sprite segSprite;
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(dRect.left, dRect.top);
				segSprite.setScale(layerScales[l], layerScales[l]);
				segSprite.setOrigin(segDef[mapSeg[i]->segmentIndex]->width / 2, segDef[mapSeg[i]->segmentIndex]->height / 2);
				segSprite.setRotation(mapSeg[i]->rotation);
				Window.draw(segSprite);
			}
		}
	}
}

void EditorScreen::DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color)
{
	sf::Rect<float> dRect;

	dRect.left = (mapSeg[segment]->position.x - scroll.x) * layerScales[mapSeg[segment]->layer];
	dRect.top = (mapSeg[segment]->position.y - scroll.y) * layerScales[mapSeg[segment]->layer];
	dRect.width = (float)segDef[mapSeg[segment]->segmentIndex]->width * layerScales[mapSeg[segment]->layer];
	dRect.height = (float)segDef[mapSeg[segment]->segmentIndex]->height * layerScales[mapSeg[segment]->layer];


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

void EditorScreen::DrawToolBar(sf::RenderWindow &Window)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(0, 0);
	sf::Vector2<float> segmentSize(1280, 40);
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(0,0,0,180));
	Window.draw(segmentShape);
}

bool EditorScreen::DrawButton(sf::RenderWindow &Window, int x, int y, int index)
{
	bool r = false;

	sf::Rect<int> sRect = sf::Rect<int>(30 * (index % 4),0,30,30);
	sf::Rect<int> dRect = sf::Rect<int>(x, y, 30, 30);

	if (dRect.contains(input.mousePos.x, input.mousePos.y))
	{
		sRect.top = 30;
		if (input.MouseButtonPressed(sf::Mouse::Button::Left))
			r = true;
	}

	sf::Sprite segSprite;
	segSprite.setTexture(toolbarIconsTex);
	segSprite.setTextureRect(sRect);
	segSprite.setPosition((float)dRect.left, (float)dRect.top);
	Window.draw(segSprite, sf::RenderStates::Default);

	return r;
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

int EditorScreen::GetHoveredSegement(sf::Vector2<int> mousePos, int layer)
{
	int hoveredSegment = -1;

	for (int i = 0; i < mapSeg.size(); i++)
	{
		if (mapSeg[i]->layer == layer)
		{
			sf::Rect<float> dRect(
				((mapSeg[i]->position.x - scroll.x) * layerScales[layer]),
				((mapSeg[i]->position.y - scroll.y) * layerScales[layer]),
				(segDef[mapSeg[i]->segmentIndex]->width * layerScales[layer]),
				(segDef[mapSeg[i]->segmentIndex]->height * layerScales[layer]));

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

void EditorScreen::SaveMap()
{
	OPENFILENAME ofn;

    TCHAR szFileName[MAX_PATH];
	szFileName[0] = '\0';

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); 
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "xml";
	if (!GetSaveFileName(&ofn)) return;

	// XML file creation
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLDeclaration* xdmap = doc->NewDeclaration();
	doc->InsertEndChild(xdmap);
	tinyxml2::XMLElement* xemap = doc->NewElement( "map" );
	tinyxml2::XMLNode* xnmap = doc->InsertEndChild(xemap);

	for (int i = 0; i < mapSeg.size(); i++)
	{
		tinyxml2::XMLElement* segment = doc->NewElement( "segment" );
		segment->SetAttribute( "id", mapSeg[i]->segmentIndex );
		segment->SetAttribute( "x", mapSeg[i]->position.x );
		segment->SetAttribute( "y", mapSeg[i]->position.y );
		segment->SetAttribute( "layer", mapSeg[i]->layer );
		segment->SetAttribute( "physics", mapSeg[i]->physicsObject );
		segment->SetAttribute( "weight", mapSeg[i]->physicsWeight );
		segment->SetAttribute( "rotation", mapSeg[i]->rotation );
		segment->SetAttribute( "scalex", mapSeg[i]->scaleX );
		segment->SetAttribute( "scaley", mapSeg[i]->scaleY );

		xemap->InsertEndChild(segment);
	}

	doc->SaveFile(szFileName);
}

void EditorScreen::LoadMap()
{
	// Reset current Map information
	ResetMap();

	OPENFILENAME ofn;

    TCHAR szFileName[MAX_PATH];
	szFileName[0] = '\0';

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); 
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "xml";
	if (!GetOpenFileName(&ofn)) return;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(szFileName);

	tinyxml2::XMLElement* root = doc.FirstChildElement( "map" );
	for (tinyxml2::XMLElement* e = root->FirstChildElement( "segment" ); e; e = e->NextSiblingElement() )
	{
		// Todo add validation routines to stop buggy XML files being imported

		int layer = std::atoi(e->Attribute("layer"));
		int segmentIndex = std::atoi(e->Attribute("id"));
		int locX = std::atoi(e->Attribute("x"));
		int locY = std::atoi(e->Attribute("y"));
		float rotation = std::atof(e->Attribute("rotation"));

		sf::Vector2<float> position(locX, locY);

		mapSeg.push_back(new MapSegment(layer, segmentIndex, position, rotation));
	}
}

void EditorScreen::ResetMap()
{
	// Empty mapSeg of all items
	mapSeg.clear();
	mouseDragSegment = -1;
	mouseHoverSegment = -1;
	mouseSelectedSegment = -1;
	scroll = sf::Vector2<float>(-640.0f, -360.0f);
	curLayer = 1;
}

std::string EditorScreen::Convert (float number)
{
	std::ostringstream buff;
	buff<<number;
	return buff.str();
}
