#include "stdafx.h"
#include "EditorScreen.h"

EditorScreen::EditorScreen(void)
{
	curLayer = 1;
	mouseDragSegment = -1;
	scroll = sf::Vector2<float>(-640.0f, -360.0f);
	zoomScale = 1.0f;
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
			if (mouseHoverSegment != -1) {
				mouseSelectedSegment = mouseHoverSegment;
			} else {
				mouseSelectedSegment = -1;
			}
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

			int xM = input.mousePos.x - input.prevMousePos.x;
			int yM = input.mousePos.y - input.prevMousePos.y;

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
				mapSeg[mouseSelectedSegment]->scale += sf::Vector2<float>((float)xM * 0.01f, (float)yM * 0.01f);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		scroll = sf::Vector2<float>(-640.0f, -360.0f);
	}

	segmentPanel->Update(input);

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

		segmentPanel->Draw(curLayer, scroll, input, Window);
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

	curZoomLevelText.setString("Zoom Level: " + Convert(zoomScale));
	curZoomLevelText.setColor(sf::Color(255,255,255,255));
	curZoomLevelText.setPosition(10, 655);
	curZoomLevelText.setFont(font);
	curZoomLevelText.setCharacterSize(14);
	Window.draw(curZoomLevelText);

	curLayerText.setString("Current Layer: " + layerName);
	curLayerText.setColor(sf::Color(255,255,255,255));
	curLayerText.setPosition(10, 675);
	curLayerText.setFont(font);
	curLayerText.setCharacterSize(14);
	Window.draw(curLayerText);

	scrollPosText.setString("Mouse Position: " + Convert(((float)input.mousePos.x + scroll.x) / zoomScale) + "," + Convert(((float)input.mousePos.y + scroll.y) / zoomScale));
	scrollPosText.setColor(sf::Color(255,255,255,255));
	scrollPosText.setPosition(10, 695);
	scrollPosText.setFont(font);
	scrollPosText.setCharacterSize(14);
	Window.draw(scrollPosText);

	std::string drawTypeName;
	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		drawTypeName = "Segment";
		break;
	case LEDGES:
		drawTypeName = "Ledges";
		break;
	}

	curDrawingMode.setString("Draw Mode: " + drawTypeName);
	curDrawingMode.setColor(sf::Color(255,255,255,255));
	curDrawingMode.setPosition(10, 635);
	curDrawingMode.setFont(font);
	curDrawingMode.setCharacterSize(14);
	Window.draw(curDrawingMode);

	DrawToolBar(Window);

	if (DrawButton(Window, 180 , 5 , 5))
	{
		zoomScale = zoomScale + 0.05;
		if (zoomScale > 2.0f) zoomScale = 2.0f;

	}
	
	if (DrawButton(Window, 145 , 5 , 4))
	{
		zoomScale = zoomScale - 0.05;
		if (zoomScale < 0.1f) zoomScale = 0.1f;


	}

	if (DrawButton(Window, 110 , 5 , 3))
	{
		curLayer = (curLayer + 1) % 3;
		mouseSelectedSegment = -1;
		mouseHoverSegment = -1;
		mouseDragSegment = -1;
		std::cout << "Layer changed to: " << curLayer << std::endl;
	}

	if (DrawButton(Window, 40 , 5 , 2))
		SaveMap();

	if (DrawButton(Window, 75 , 5 , 1))
		LoadMap();

	if (DrawButton(Window, 5 , 5 , 0))
		ResetMap();

	sf::Sprite toolbarSpacer;
	toolbarSpacer.setTexture(toolbarIconsTex);
	toolbarSpacer.setTextureRect(sf::Rect<int>(0, 2, 2, 30));
	toolbarSpacer.setPosition(220, 5);
	Window.draw(toolbarSpacer, sf::RenderStates::Default);

	for ( int i = SEGMENT_SELECTION; i != DMODE_LAST; i++ )
	{
		if (DrawButton(Window, 232 + (i * 35) , 5 , (6 + i))) {
			drawingMode = (drawingMode_t)i;
		}
	}
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
				dRect.left = (mapSeg[i]->position.x - scroll.x) * (layerScales[l] * zoomScale);
				dRect.top = (mapSeg[i]->position.y - scroll.y) * (layerScales[l] * zoomScale);
				dRect.width = (float)segDef[mapSeg[i]->segmentIndex]->width;
				dRect.height = (float)segDef[mapSeg[i]->segmentIndex]->height;

				sf::Sprite segSprite;
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(dRect.left, dRect.top);
				segSprite.setScale((layerScales[l] * zoomScale) * mapSeg[i]->scale.x, (layerScales[l] * zoomScale) * mapSeg[i]->scale.y);
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

	dRect.left = (mapSeg[segment]->position.x - scroll.x) * (layerScales[mapSeg[segment]->layer] * zoomScale);
	dRect.top = (mapSeg[segment]->position.y - scroll.y) * (layerScales[mapSeg[segment]->layer] * zoomScale);
	dRect.width = (float)segDef[mapSeg[segment]->segmentIndex]->width * (layerScales[mapSeg[segment]->layer] * zoomScale) * mapSeg[segment]->scale.x;
	dRect.height = (float)segDef[mapSeg[segment]->segmentIndex]->height * (layerScales[mapSeg[segment]->layer] * zoomScale) * mapSeg[segment]->scale.y;


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

	sf::Rect<int> sRect = sf::Rect<int>(2 + (30 * (index % 9)),0,30,30);
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
				((mapSeg[i]->position.x - scroll.x) * (layerScales[layer] * zoomScale)),
				((mapSeg[i]->position.y - scroll.y) * (layerScales[layer] * zoomScale)),
				(segDef[mapSeg[i]->segmentIndex]->width * (layerScales[layer] * zoomScale) * mapSeg[i]->scale.x),
				(segDef[mapSeg[i]->segmentIndex]->height * (layerScales[layer] * zoomScale) * mapSeg[i]->scale.y));

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
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
	
    if (SUCCEEDED(hr))
    {
		IFileSaveDialog *pFileSave = NULL;

		HRESULT hr = CoCreateInstance(__uuidof(FileSaveDialog), NULL, 
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSave));

		if (SUCCEEDED(hr))
		{
			hr = pFileSave->SetDefaultExtension(L"xml");
			hr = pFileSave->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
			hr = pFileSave->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileSave->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						std::string filePath = utf8_encode(pszFilePath);
						char *a=new char[filePath.size()+1];
						a[filePath.size()]=0;
						memcpy(a,filePath.c_str(),filePath.size());

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
							segment->SetAttribute( "scalex", mapSeg[i]->scale.x );
							segment->SetAttribute( "scaley", mapSeg[i]->scale.y );

							xemap->InsertEndChild(segment);
						}

						doc->SaveFile(a);

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
}

void EditorScreen::LoadMap()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
	
    if (SUCCEEDED(hr))
    {
		IFileOpenDialog *pFileOpen = NULL;

		HRESULT hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, 
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->SetDefaultExtension(L"xml");
			hr = pFileOpen->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						// Reset current Map information
						ResetMap();

						std::string filePath = utf8_encode(pszFilePath);
						char *a=new char[filePath.size()+1];
						a[filePath.size()]=0;
						memcpy(a,filePath.c_str(),filePath.size());

						tinyxml2::XMLDocument doc;
						doc.LoadFile(a);

						tinyxml2::XMLElement* root = doc.FirstChildElement( "map" );
						for (tinyxml2::XMLElement* e = root->FirstChildElement( "segment" ); e; e = e->NextSiblingElement() )
						{
							// Todo add validation routines to stop buggy XML files being imported

							int layer = std::atoi(e->Attribute("layer"));
							int segmentIndex = std::atoi(e->Attribute("id"));
							int locX = std::atoi(e->Attribute("x"));
							int locY = std::atoi(e->Attribute("y"));
							float rotation = std::atof(e->Attribute("rotation"));
							float scaleX = std::atof(e->Attribute("scalex"));
							float scaleY = std::atof(e->Attribute("scaley"));

							sf::Vector2<float> position(locX, locY);
							sf::Vector2<float> scale(scaleX, scaleY);

							if(segmentIndex < segDef.size()) {
								std::cout << "Loading segment index: " << segmentIndex << std::endl;
								mapSeg.push_back(new MapSegment(layer, segmentIndex, position, rotation, scale));
							} else {
								std::cout << "Cannot load segment index: " << segmentIndex << std::endl;
							}
						}

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}

void EditorScreen::ResetMap()
{
	// Empty mapSeg of all items
	mapSeg.clear();
	mouseDragSegment = -1;
	mouseHoverSegment = -1;
	mouseSelectedSegment = -1;
	zoomScale = 1.0f;
	scroll = sf::Vector2<float>(-640.0f, -360.0f);
	curLayer = 1;
	drawingMode = SEGMENT_SELECTION;
}

std::string EditorScreen::Convert (float number)
{
	std::ostringstream buff;
	buff<<number;
	return buff.str();
}

// Convert a wide Unicode string to an UTF8 string
std::string EditorScreen::utf8_encode(const std::wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
