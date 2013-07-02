#include "stdafx.h"
#include "UI_CharPreview.h"
#include "CharacterEditorScreen.h"

UICharPreview::UICharPreview(CharDef *&charDef, std::vector<sf::Texture> &legsTex, std::vector<sf::Texture> &torsoTex, std::vector<sf::Texture> &headTex, int &selAnim, int &selKeyFrame) 
	: legsTex(legsTex), torsoTex(torsoTex), headTex(headTex), selAnim(selAnim), selKeyFrame(selKeyFrame), Panel()
{
	this->charDef = charDef;
	this->title = "Preview";
	this->position = sf::Rect<float>(860,50,200,300);
	this->isResizable = false;
	this->isCloseable = false;

	this->legsTex = legsTex;
	this->torsoTex = torsoTex;
	this->headTex = headTex;

	FACE_LEFT = 0;
	FACE_RIGHT = 1;

	curKey = 0;
	curFrame = 0;
}


UICharPreview::~UICharPreview(void)
{

}

void UICharPreview::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);

	Animation *animation = charDef->animations[selAnim];

	if (animation->keyFrames.size() > 0)
	{
		KeyFrame *keyframe = animation->keyFrames[curKey];

		sf::Time elapsed = gameTime.getElapsedTime();

		if (playing)
		{
			curFrame += (float)elapsed.asSeconds() * 30.f;

			if (curFrame > (float)keyframe->duration)
			{
				curFrame -= (float)keyframe->duration;
				curKey++;

				if (curKey >= animation->keyFrames.size())
					curKey = 0;

				keyframe = animation->keyFrames[curKey];
			}
		}
		else
			curKey = selKeyFrame;

		if (keyframe->frameRef < 0)
			curKey = 0;
	}

}

void UICharPreview::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	if (charDef->animations[selAnim]->keyFrames.size() > 0)
	{

		int fref = charDef->animations[selAnim]->keyFrames[curKey]->frameRef;
		if (fref < 0)
			fref = 0;

		DrawCharacter(sf::Vector2f(position.left + 30, position.top + 150), 1.0f, FACE_RIGHT, fref, true, 255.f, Window);

		text.setFont(font);
		text.setCharacterSize(12);
		text.setPosition(position.left + 10, (position.top + position.height) - 10);

		if (playing)
		{
			text.setString("stop");
		}
		else
		{
			text.setString("play");
		}
	}
}

void UICharPreview::DrawCharacter(sf::Vector2f loc, float scale, int face, int frameIndex, bool preview, float alpha, sf::RenderWindow &Window)
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
		if (part->index >= 128) scaling *= 1.35f;

		if (face == 0)
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
		sprite.setColor(sf::Color(255,255,255,alpha));
		Window.draw(sprite);

	}
}