#include "stdafx.h"
#include "CharacterEditorScreen.h"

CharacterEditorScreen::CharacterEditorScreen(void)
{
	FACE_LEFT = 0;
	FACE_RIGHT = 1;
	selPart = 0;
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

	charPalettePane = new UICharPalettePanel(charDef);
	panelManager->AddPanel(charPalettePane);
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

	pMousePos = mousePos;

	// Change Editor
	if (InputManager::instance().Pressed(sf::Keyboard::Tab))
	{
		ScreenManager::GetInstance().AddScreen(new EditorScreen);
	}
}

void CharacterEditorScreen::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	DrawCharacter(sf::Vector2f(400.0f, 450.0f), 2.0f, FACE_RIGHT, selFrame, false, 1.0f, Window);
	DrawPartsList(Window);
	DrawPalette(Window);
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

		sf::Vector2f location = part->location * scale + loc;
		sf::Vector2f scaling = part->scaling * scale;

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
		//sprite.setRotation(rotation);
		Window.draw(sprite);

	}
}

void CharacterEditorScreen::DrawPalette(sf::RenderWindow &Window)
{
	for (int l = 0; l < 3; l++)
	{
		sf::Sprite sprite;

		switch (l)
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

		for (int i = 0; i < 25; i++)
		{
			sf::Rect<int> sRect;
			sRect.left = (i % 5) * 64;
			sRect.top = (i / 5) * 64;
			sRect.width = 64;
			sRect.height = 64;

			// draw
			sprite.setTextureRect(sRect);
			sprite.setOrigin(32.0f, 32.0f);
			sprite.setPosition(i * 23, 467 + (l * 32));
			sprite.setScale(.5f, .5f);
			Window.draw(sprite);

			if (mousePos.x > (i * 23) && mousePos.x < ((i * 23) + 32) &&
				mousePos.y > (467 + (l * 32)) && mousePos.y < ((467 + (l * 32)) + 32))
			{
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					charDef->frames[selFrame]->parts.push_back(new Part(i + 64 * l));
					std::cout << "Added new part: " << (i + 64 * l) << std::endl;
				}
			}
		}
	}
}

void CharacterEditorScreen::DrawPartsList(sf::RenderWindow &Window)
{
	text.setFont(font);
	text.setCharacterSize(12);
	text.setColor(sf::Color::White);
	
	for (int i = 0; i < charDef->frames[selFrame]->parts.size(); i++)
	{
		int y = 5 + i * 15;
		int index = charDef->frames[selFrame]->parts[i]->index;
		
		if (index < 64)
			text.setString("head " + Convert(index));
		else if (index < 74)
			text.setString("torso " + Convert(index));
		else if (index < 128)
			text.setString("arms " + Convert(index));
		else
			text.setString("legs " + Convert(index));

		text.setPosition(600, y);

		if (selPart == i)
		{

		}
		
		Window.draw(text);
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