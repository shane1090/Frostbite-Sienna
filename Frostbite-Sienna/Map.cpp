#include "stdafx.h"
#include "Map.h"

Map::Map(void)
{
	zoomScale = 1.0f;
}


Map::~Map(void)
{

}

int Map::GetLedgeSec(int l, float x)
{
	for (int i = 0; i < ledges[l]->nodes.size() - 1; i++)
	{
		if (x >= ledges[l]->nodes[i].x &&
			x <= ledges[l]->nodes[i + 1].x)
			return i;
	}

	return -1;
}

float Map::GetLedgeYLoc(int l, int i, float x)
{
	return (ledges[l]->nodes[i + 1].y - ledges[l]->nodes[i].y) *
		((x - ledges[l]->nodes[i].x) /
			(ledges[l]->nodes[i + 1].x - ledges[l]->nodes[i].x)
			) + ledges[l]->nodes[i].y;
}

void Map::Draw(sf::RenderWindow &Window, int startLayer, int endLayer, sf::Vector2<float> &scroll)
{
	for (int l = startLayer; l < endLayer; l++)
	{
		float scale = 1.0f;
		if (l == 0)
		{
			scale = 0.75f;
		}
		else if (l == 2)
		{
			scale = 1.25f;
		}

		for (int i = 0; i < mapSeg.size(); i++)
		{
			if (mapSeg[i]->layer == l)
			{
				sf::Rect<float> dRect;
				dRect.left = (mapSeg[i]->position.x - scroll.x) * (scale * zoomScale);
				dRect.top = (mapSeg[i]->position.y - scroll.y) * (scale * zoomScale);
				dRect.width = (float)segDef[mapSeg[i]->segmentIndex]->width;
				dRect.height = (float)segDef[mapSeg[i]->segmentIndex]->height;

				sf::Sprite segSprite;
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(dRect.left, dRect.top);
				segSprite.setScale((scale * zoomScale) * mapSeg[i]->scale.x, (scale * zoomScale) * mapSeg[i]->scale.y);
				segSprite.setOrigin(segDef[mapSeg[i]->segmentIndex]->width / 2, segDef[mapSeg[i]->segmentIndex]->height / 2);
				segSprite.setRotation(mapSeg[i]->rotation);
				Window.draw(segSprite);
			}
		}
	}
}

void Map::LoadConfig(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges)
{
	this->segDef = segDef;
	this->mapSeg = mapSeg;
	this->ledges = ledges;
}