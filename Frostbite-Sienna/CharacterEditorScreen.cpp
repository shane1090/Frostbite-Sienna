#include "stdafx.h"
#include "CharacterEditorScreen.h"

CharacterEditorScreen::CharacterEditorScreen(void)
{
	FACE_LEFT = 0;
	FACE_RIGHT = 1;
	selPart = -1;
	selFrame = 0;

	charDef = new CharDef;

	// Generate initial Frame
	charDef->frames.push_back(new Frame);
}

CharacterEditorScreen::~CharacterEditorScreen(void)
{

}

void CharacterEditorScreen::LoadContent()
{
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;

	if (!toolbarIconsTex.loadFromFile("Assets/GUI/editor-icons.png"))
		std::cout << "Could not load toolbar icons" << std::endl;

	LoadTextures(headTex, "Assets/characters/head");
	LoadTextures(torsoTex, "Assets/characters/torso");
	LoadTextures(legsTex, "Assets/characters/legs");

	// Intiliase panelManager
	panelManager = new PanelManager();

	charPalettePane = new UICharPalettePanel(charDef, selFrame, legsTex, torsoTex, headTex);
	panelManager->AddPanel(charPalettePane);

	charPartsPane = new UICharPartsList(charDef, selFrame, selPart);
	panelManager->AddPanel(charPartsPane);
}

void CharacterEditorScreen::LoadTextures(std::vector<sf::Texture> &textures, std::string path)
{
	for (int i = 0; i < 1; i++)
	{
		sf::Texture newTex;
		if (!newTex.loadFromFile(path + std::to_string(i + 1) + ".png"))
		{
			std::cout << "Could not load texture: " << path << i << std::endl;
		}
		else
		{
			textures.push_back(newTex);
		}
	}
}

void CharacterEditorScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void CharacterEditorScreen::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	InputManager::instance().Poll(Window);

	mousePos = InputManager::instance().getMousePosition();

	int xM = mousePos.x - pMousePos.x;
	int yM = mousePos.y - pMousePos.y;

	if (!panelManager->CheckMouseHover(mousePos))
	{
		mouseHoverPart = GetHoveredPart(mousePos);
		
		if (selPart != -1 && mouseHoverPart == selPart)
		{
			if (InputManager::instance().HeldDown(sf::Mouse::Button::Left, true))
			{
				charDef->frames[selFrame]->parts[selPart]->location +=
					sf::Vector2f((float)xM, (float)yM);
			}
		}

		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
		{
			if (mouseHoverPart != -1) {
				selPart = mouseHoverPart;
			} else {
				selPart = -1;
			}
		}
	}
	




	panelManager->Update(Window, gameTime);

	pMousePos = mousePos;

	// Change Editor
	if (InputManager::instance().Pressed(sf::Keyboard::Tab))
	{
		ScreenManager::GetInstance().AddScreen(new EditorScreen);
	}
}

void CharacterEditorScreen::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	DrawCharacter(sf::Vector2f(400.0f, 250.0f), 2.0f, FACE_RIGHT, selFrame, false, 1.0f, Window);

	if (mouseHoverPart > -1)
		DrawSelectedPart(Window, mouseHoverPart, sf::Color::White);

	if (selPart > -1)
	{
		DrawSelectedPart(Window, selPart, sf::Color::Red);
	}

	panelManager->Draw(Window, gameTime);

	DrawToolBar(Window);
}

void CharacterEditorScreen::DrawCharacter(sf::Vector2f loc, float scale, int face, int frameIndex, bool preview, float alpha, sf::RenderWindow &Window)
{
	Frame *frame = charDef->frames[frameIndex];

	for (int i = 0; i < frame->parts.size(); i++)
	{
		Part *part = frame->parts[i];

		sf::Rect<int> sRect;
		sRect.left = ((part->index % 64) % 5) * 64;
		sRect.top = ((part->index % 64) / 5) * 64;
		sRect.width = 64;
		sRect.height = 64;

		float rotation = part->rotation;

		sf::Vector2f location = part->location + loc;
		sf::Vector2f scaling = part->scaling * scale;
		if (part->index >= 128) scaling *= 1.35f;

		if (face == FACE_LEFT)
		{
			rotation = -rotation;
			location.x -= part->location.x * scale * 2.0f;
		}

		sf::Sprite sprite;

		int t = part->index / 64;
		switch (t)
		{
			case 0:
				sprite.setTexture(headTex[charDef->headIndex]);
				break;
			case 1:
				sprite.setTexture(torsoTex[charDef->torsoIndex]);
				break;
			case 2:
				sprite.setTexture(legsTex[charDef->legsIndex]);
				break;
		}

		bool flip = false;

		if ((face == FACE_RIGHT && part->flip == 0) ||
			(face == FACE_LEFT && part->flip == 1))
			flip = true;

		// draw
		sprite.setTextureRect(sRect);
		sprite.setOrigin(32.0f, 32.0f);
		sprite.setPosition(location.x, location.y);
		sprite.setScale(scaling.x, scaling.y);
		sprite.setRotation(rotation);
		Window.draw(sprite);

	}
}

void CharacterEditorScreen::DrawToolBar(sf::RenderWindow &Window)
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

	if (DrawButton(Window, x, 5 , 0)) // New
	{
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 2)) // Save
	{
	}

	x = x + 35;
	if (DrawButton(Window, x, 5 , 1)) // Load
	{
	}
}

bool CharacterEditorScreen::DrawButton(sf::RenderWindow &Window, int x, int y, int index)
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

void CharacterEditorScreen::DrawSelectedPart(sf::RenderWindow &Window, int part, sf::Color color)
{
	sf::Rect<float> dRect;

	dRect.left = charDef->frames[selFrame]->parts[part]->location.x + 400.0f;
	dRect.top = charDef->frames[selFrame]->parts[part]->location.y + 250.0f;
	dRect.width = 64.0f * 2.0f;
	dRect.height = 64.0f * 2.0f;


	sf::RectangleShape segmentShape;
	segmentShape.setPosition(dRect.left, dRect.top);
	segmentShape.setRotation(charDef->frames[selFrame]->parts[part]->rotation);
	segmentShape.setOrigin(dRect.width / 2, dRect.height / 2);
	
	sf::Vector2<float> segmentSize(dRect.width, dRect.height);
	
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color::Transparent);
	segmentShape.setOutlineColor(color);
	segmentShape.setOutlineThickness(1);
	Window.draw(segmentShape);
}

int CharacterEditorScreen::GetHoveredPart(sf::Vector2<int> mousePos)
{
	int hoveredPart = -1;

	for (int i = 0; i < charDef->frames[selFrame]->parts.size(); i++)
	{
		sf::Rect<float> dRect(
			(charDef->frames[selFrame]->parts[i]->location.x + 400.0f),
			(charDef->frames[selFrame]->parts[i]->location.y + 250.0f),
			64 * 2.0f,
			64 * 2.0f);

		float c = cos(-charDef->frames[selFrame]->parts[i]->rotation * M_PI / 180);
		float s = sin(-charDef->frames[selFrame]->parts[i]->rotation * M_PI / 180);

		float rotatedX = dRect.left + c * (mousePos.x - dRect.left) - s * (mousePos.y - dRect.top);
		float rotatedY = dRect.top + s * (mousePos.x - dRect.left) + c * (mousePos.y - dRect.top);

		float leftX = dRect.left - dRect.width / 2;
		float rightX = dRect.left + dRect.width / 2;
		float topY = dRect.top - dRect.height / 2;
		float bottomY = dRect.top + dRect.height / 2;

		if (leftX <= rotatedX && rotatedX <= rightX &&
			topY <= rotatedY && rotatedY <= bottomY)
		{
			hoveredPart = i;
		}
	}

	return hoveredPart;
}