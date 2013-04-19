#include "stdafx.h"
#include "EditorScreen.h"

EditorScreen::EditorScreen(void)
{
	curLayer = 1;
	curLedge = 0;
	mouseDragSegment = -1;
	mouseSelectedSegment = -1;
	scroll = sf::Vector2<float>(-640.0f, -360.0f); // Set 0,0 to center of screen
	zoomScale = 1.0f;
	scrollMap = false;
	drawingMode = SEGMENT_SELECTION;
	tile = -1;
	map = NULL;
}

EditorScreen::~EditorScreen(void)
{

}

void EditorScreen::LoadContent()
{
	// Initiate map class
	if (!map)
	{
		map = new Map;
		map->LoadSegmentDefinitions();
	}
	
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;

	if (!toolbarIconsTex.loadFromFile("Assets/GUI/editor-icons.png"))
		std::cout << "Could not load toolbar icons" << std::endl;

	drawingMode = SEGMENT_SELECTION;

	// Intiliase panelManager
	panelManager = new PanelManager();
	segmentPane = new UISegmentPanel(map, tile);
	segmentPane->minimized = true;
	panelManager->AddPanel(segmentPane);

	segmentInfoPanel = new SegmentInfoPanel(segDef, mapSeg);
	segmentInfoPanel->LoadContent(font);

	ledgePanel = new LedgePanel(ledges);
	ledgePanel->LoadContent(font);

	// Set parallax scrolling scales for each layer
	layerScales.push_back(0.75f);
	layerScales.push_back(1.0f);
	layerScales.push_back(1.0f);
	layerScales.push_back(1.0f);
	layerScales.push_back(1.25f);
}

void EditorScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void EditorScreen::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	InputManager::instance().Poll(Window);

	mousePos = InputManager::instance().getMousePosition();

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:

		if (mousePos.y > 40)
		{
			if (!panelManager->CheckMouseHover(mousePos))
			{
				if (tile != -1)
				{
					if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
					{
						// Add segment here
						sf::Vector2<float> position((mousePos.x + scroll.x), (mousePos.y + scroll.y));
						std::cout << "Segment Added to Map: " << tile << std::endl;
						map->mapSeg.push_back(new MapSegment(curLayer, tile, position, 0, sf::Vector2<float>(1.0f,1.0f)));

						// Reset tile - this needs to change so multiple tiles can be placed
						// but need to change the checks when placing them so they can be positioned over
						// existing segments
						tile = -1;
					}
				}
				else
				{

					mouseHoverSegment = GetHoveredSegement(mousePos, curLayer);
			
					if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true) && mousePos.x < SCREEN_WIDTH && mouseDragSegment == -1)
						if (mouseHoverSegment != -1)
							mouseDragSegment = mouseHoverSegment;

					if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
					{
						if (mouseHoverSegment != -1) {
							mouseSelectedSegment = mouseHoverSegment;
						} else {
							mouseSelectedSegment = -1;
						}
					}
				}
			} 
			else 
			{
				mouseHoverSegment = -1;
			}
		
			if (mouseDragSegment > -1 && mouseSelectedSegment > -1)
			{
				if (InputManager::instance().Released(sf::Mouse::Button::Left, true))
					mouseDragSegment = -1;
				else
				{
					sf::Vector2<float> loc = map->mapSeg[mouseDragSegment]->position;
					loc.x += (mousePos.x - pMousePos.x) / layerScales[curLayer];
					loc.y += (mousePos.y - pMousePos.y) / layerScales[curLayer];
					map->mapSeg[mouseDragSegment]->position = loc;
				}
			}

			
		}
		else
		{
			mouseHoverSegment = -1;
		}

		if (mouseSelectedSegment != -1)
		{
			// Allow deletion of segments
			if (InputManager::instance().Pressed(sf::Keyboard::Delete))
			{
				std::cout << "Map segment deleted: " << mouseSelectedSegment << std::endl;
				map->mapSeg.erase(map->mapSeg.begin()+mouseSelectedSegment);
				mouseSelectedSegment = -1;
				mouseHoverSegment = -1;
			}

			int xM = mousePos.x - pMousePos.x;
			int yM = mousePos.y - pMousePos.y;

			// Allow rotation of segments
			if (InputManager::instance().Pressed(sf::Keyboard::R))
			{
				float angle = atan2((mousePos.y - map->mapSeg[mouseSelectedSegment]->position.y) + scroll.y,
								    (mousePos.x - map->mapSeg[mouseSelectedSegment]->position.x) + scroll.x);

				map->mapSeg[mouseSelectedSegment]->rotation = angle * (180.0f/M_PI);
			}

			// Allow scaling of segments
			if (InputManager::instance().Pressed(sf::Keyboard::S))
			{
				map->mapSeg[mouseSelectedSegment]->scale += sf::Vector2<float>((float)xM * 0.05f, (float)yM * 0.05f);
			}
		}

		break;
	case LEDGES:

		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
		{
			if (mousePos.x > 0 && mousePos.x < SCREEN_WIDTH &&
				mousePos.y > 40 && mousePos.y < 720 &&
				!ledgePanel->panelRect.contains(mousePos.x, mousePos.y))
			{
				if (ledges.size() == 0)
					ledges.push_back(new Ledge);

				sf::Vector2<float> nodePos = sf::Vector2<float>(mousePos.x, mousePos.y) + scroll;

				ledges[curLedge]->nodes.push_back(nodePos);

				std::cout << "Node added to ledge " << curLedge << ": " << nodePos.x << ", " << nodePos.y << std::endl;
			}
		}

		// Add new ledge
		if (InputManager::instance().Pressed(sf::Keyboard::A))
		{
			ledges.push_back(new Ledge);
			curLedge = ledges.size() - 1;
		}

		// Allow deletion of nodes in ledges
		if (InputManager::instance().Pressed(sf::Keyboard::Delete))
		{
			if (ledges[curLedge]->nodes.size() > 1)
			{
				std::cout << "Ledge " << curLedge << " node deleted" << std::endl;
				ledges[curLedge]->nodes.erase(ledges[curLedge]->nodes.end() - 1);
				
			} else {
				ledges.erase(ledges.begin() + curLedge);
				std::cout << "Ledge " << curLedge << " deleted" << std::endl;
				if (ledges.size())
					curLedge = curLedge - 1;
				else
					curLedge = -1;
			}
		}

		ledgePanel->Update();

		break;
	}

	if (InputManager::instance().Pressed(sf::Mouse::Button::Right, true))
	{
		scrollMap = true;
	}

	if (scrollMap)
	{
		if (InputManager::instance().Released(sf::Mouse::Button::Right, true))
			scrollMap = false;

		scroll.x -= (mousePos.x - pMousePos.x) * 2.0f;
		scroll.y -= (mousePos.y - pMousePos.y) * 2.0f;
	}

	if (InputManager::instance().Pressed(sf::Keyboard::C))
	{
		scroll = sf::Vector2<float>(-640.0f, -360.0f);
	}

	if (InputManager::instance().Pressed(sf::Keyboard::A))
	{
		segmentPane->minimized = !segmentPane->minimized;
	}

	panelManager->Update(Window, gameTime);

	pMousePos = mousePos;
}

void EditorScreen::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	map->Draw(Window, 0, 5, scroll);

	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		if (mouseHoverSegment > -1)
			DrawSelectedSegment(Window, mouseHoverSegment, sf::Color::White);

		if (mouseSelectedSegment > -1)
		{
			DrawSelectedSegment(Window, mouseSelectedSegment, sf::Color::Red);
			//segmentInfoPanel->Draw(Window, mouseSelectedSegment);
		}

		if (tile != -1)
		{
			sf::Sprite segSprite;
			segSprite.setTexture(map->segDef[tile]->tex);
			segSprite.setPosition(mousePos.x, mousePos.y);
			segSprite.setScale((layerScales[curLayer] * zoomScale), (layerScales[curLayer] * zoomScale));
			segSprite.setOrigin(map->segDef[tile]->width / 2, map->segDef[tile]->height / 2);
			segSprite.setColor(sf::Color(255,255,255,100));
			Window.draw(segSprite);
		}
		
		break;
	case LEDGES:
		map->DrawLedges(Window, scroll, curLedge);
		ledgePanel->Draw(Window, curLedge);
		break;
	}

	std::string layerName = "map";
	switch (curLayer)
	{
	case 0:
		layerName = "back";
		break;
	case 1:
		layerName = "mid-back";
		break;
	case 2:
		layerName = "mid-center";
		break;
	case 3:
		layerName = "mid-fore";
		break;
	case 4:
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

	scrollPosText.setString("Mouse Position: " + Convert(((float)mousePos.x + scroll.x) / zoomScale) + "," + Convert(((float)mousePos.y + scroll.y) / zoomScale));
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

	panelManager->Draw(Window, gameTime);

	DrawToolBar(Window);
}

void EditorScreen::DrawSelectedSegment(sf::RenderWindow &Window, int segment, sf::Color color)
{
	sf::Rect<float> dRect;

	dRect.left = (map->mapSeg[segment]->position.x - scroll.x) * (layerScales[map->mapSeg[segment]->layer] * zoomScale);
	dRect.top = (map->mapSeg[segment]->position.y - scroll.y) * (layerScales[map->mapSeg[segment]->layer] * zoomScale);
	dRect.width = (float)map->segDef[map->mapSeg[segment]->segmentIndex]->width * (layerScales[map->mapSeg[segment]->layer] * zoomScale) * map->mapSeg[segment]->scale.x;
	dRect.height = (float)map->segDef[map->mapSeg[segment]->segmentIndex]->height * (layerScales[map->mapSeg[segment]->layer] * zoomScale) * map->mapSeg[segment]->scale.y;


	sf::RectangleShape segmentShape;
	segmentShape.setPosition(dRect.left, dRect.top);
	segmentShape.setRotation(map->mapSeg[segment]->rotation);
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

	sf::Sprite toolbarSpacer;
	toolbarSpacer.setTexture(toolbarIconsTex);
	toolbarSpacer.setTextureRect(sf::Rect<int>(0, 2, 2, 30));

	int x = 5;

	if (DrawButton(Window, x, 5 , 0)) // New Map
		ResetMap();

	x = x + 35;
	if (DrawButton(Window, x, 5 , 2)) // Save Map
		SaveMap();

	x = x + 35;
	if (DrawButton(Window, x, 5 , 1)) // Load Map
		LoadMap();

	x = x + 35;
	if (DrawButton(Window, x, 5 , 3)) // Layer change
	{
		curLayer = (curLayer + 1) % 5;
		mouseSelectedSegment = -1;
		mouseHoverSegment = -1;
		mouseDragSegment = -1;
		std::cout << "Layer changed to: " << curLayer << std::endl;
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 4)) // Zoom out
	{
		zoomScale = zoomScale - 0.05;
		if (zoomScale < 0.1f) zoomScale = 0.1f;
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 5)) // Zoom in
	{
		zoomScale = zoomScale + 0.05;
		if (zoomScale > 2.0f) zoomScale = 2.0f;
	}

	x = x + 40;
	toolbarSpacer.setPosition(x, 5);
	Window.draw(toolbarSpacer, sf::RenderStates::Default);
	
	x = x + 10;
	if (DrawButton(Window, x, 5 , 6)) // Segment drawing mode
	{
		drawingMode = (drawingMode_t)SEGMENT_SELECTION;
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 7)) // Ledge drawing mode
	{
		drawingMode = (drawingMode_t)LEDGES;
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 8)) // Collision drawing mode
	{
		
	}

	x = x + 40;
	toolbarSpacer.setPosition(x, 5);
	Window.draw(toolbarSpacer, sf::RenderStates::Default);

	x = x + 10;
	if (DrawButton(Window, x, 5 , 9)) // Test map
	{
		PlayTestScreen *screen = new PlayTestScreen;
		screen->SetMapData(map);
		ScreenManager::GetInstance().AddScreen(screen);
	}

}

bool EditorScreen::DrawButton(sf::RenderWindow &Window, int x, int y, int index)
{
	bool r = false;

	sf::Rect<int> sRect = sf::Rect<int>(2 + (30 * (index % 10)),0,30,30);
	sf::Rect<int> dRect = sf::Rect<int>(x, y, 30, 30);

	if (dRect.contains(mousePos.x, mousePos.y))
	{
		sRect.top = 30;

		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			r = true;
	}
	
	sf::Sprite segSprite;
	segSprite.setTexture(toolbarIconsTex);
	segSprite.setTextureRect(sRect);
	segSprite.setPosition((float)dRect.left, (float)dRect.top);
	Window.draw(segSprite, sf::RenderStates::Default);

	return r;
}

int EditorScreen::GetHoveredSegement(sf::Vector2<int> mousePos, int layer)
{
	int hoveredSegment = -1;

	for (int i = 0; i < map->mapSeg.size(); i++)
	{
		if (map->mapSeg[i]->layer == layer)
		{
			sf::Rect<float> dRect(
				((map->mapSeg[i]->position.x - scroll.x) * (layerScales[layer] * zoomScale)),
				((map->mapSeg[i]->position.y - scroll.y) * (layerScales[layer] * zoomScale)),
				(map->segDef[map->mapSeg[i]->segmentIndex]->width * (layerScales[layer] * zoomScale) * map->mapSeg[i]->scale.x),
				(map->segDef[map->mapSeg[i]->segmentIndex]->height * (layerScales[layer] * zoomScale) * map->mapSeg[i]->scale.y));

			float c = cos(-map->mapSeg[i]->rotation * M_PI / 180);
			float s = sin(-map->mapSeg[i]->rotation * M_PI / 180);

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

int EditorScreen::GetHoveredLedgeNode(sf::Vector2<int> mousePos)
{
	int hoveredNode = -1;

	return hoveredNode;
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
						
						map->SaveMap(filePath);

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
						map->LoadMap(filePath);

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
	mouseDragSegment = -1;
	mouseHoverSegment = -1;
	mouseSelectedSegment = -1;
	zoomScale = 1.0f;
	scroll = sf::Vector2<float>(-640.0f, -360.0f);
	curLayer = 1;

	map->ledges.clear();
	map->mapSeg.clear();

	curLedge = 0;
}

void EditorScreen::SetMapData(Map *map)
{
	this->map = map;
}
