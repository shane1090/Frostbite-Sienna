#include "stdafx.h"
#include "Map.h"

Map::Map(void)
{
	LoadDefaultLayers();
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
		for (int i = 0; i < mapSeg.size(); i++)
		{
			if (mapSeg[i]->layer == l)
			{
				sf::Vector2f position;

				position.x = mapSeg[i]->position.x - (scroll.x * layers[mapSeg[i]->layer]->scale);
				position.y = mapSeg[i]->position.y - (scroll.y * layers[mapSeg[i]->layer]->scale);

				sf::Sprite segSprite;
				segSprite.setOrigin((float)segDef[mapSeg[i]->segmentIndex]->width / 2.0f, (float)segDef[mapSeg[i]->segmentIndex]->height / 2.0f);
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(position.x, position.y);
				segSprite.setScale(mapSeg[i]->scale.x, mapSeg[i]->scale.y);
				segSprite.setRotation(mapSeg[i]->rotation);
				Window.draw(segSprite);
			}
		}
	}
}

void Map::DrawLedges(sf::RenderWindow &Window, sf::Vector2<float> &scroll, int curLedge)
{
	sf::Rect<int> rect;
	sf::Color tColor;

	for (int i = 0; i < ledges.size(); i++)
	{
		if (ledges[i]->nodes.size() > 0)
		{
			sf::VertexArray lines(sf::LinesStrip, ledges[i]->nodes.size());

			for (int n = 0; n < ledges[i]->nodes.size(); n++)
			{
				// Change colour if ledge is selected
				if (curLedge == i)
					tColor = sf::Color::Yellow;
				else
					tColor = sf::Color::White;

				sf::Vector2<float> tVec;
				tVec = ledges[i]->nodes[n];
				tVec -= scroll;

				lines[n].position = tVec;
				lines[n].color = tColor;

				tVec.x -= 2.0f;
				tVec.y -= 2.0f;
				
				// Change colour if ledge is selected
				if (curLedge == i)
					lines[n].color = sf::Color::Yellow;
				else
					lines[n].color = sf::Color::White;

				sf::CircleShape circle;
				circle.setRadius(4);
				circle.setFillColor(tColor);
				circle.setPosition(tVec.x, tVec.y);
				circle.setOrigin(2,2);
				Window.draw(circle);
			}

			Window.draw(lines);
		}
	}
}

void Map::LoadDefaultLayers()
{
	layers.push_back(new Layer("Background", 0.75f));
	layers.push_back(new Layer("Close Background", 1.0f));
	layers.push_back(new Layer("Collision Layer", 1.0f));
	layers.push_back(new Layer("Close Foreground", 1.0f));
	layers.push_back(new Layer("Foreground", 1.25f));
}

void Map::LoadSegmentDefinitions()
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

void Map::LoadConfig(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges)
{
	this->segDef = segDef;
	this->mapSeg = mapSeg;
	this->ledges = ledges;
}

void Map::LoadMap(std::string filePath)
{
	char *a=new char[filePath.size()+1];
	a[filePath.size()]=0;
	memcpy(a,filePath.c_str(),filePath.size());
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a);

	tinyxml2::XMLElement* xemap = doc.FirstChildElement( "map" );
	for (tinyxml2::XMLElement* e = xemap->FirstChildElement( "segment" ); e; e = e->NextSiblingElement() )
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
	
	tinyxml2::XMLElement* xeledges = doc.FirstChildElement( "ledges" );
	
	if (xeledges != NULL)
	{
		int i = 0;
		for (tinyxml2::XMLElement* e = xeledges->FirstChildElement( "ledge" ); e; e = e->NextSiblingElement() )
		{
			ledges.push_back(new Ledge);
			
			int flag = std::atoi(e->Attribute("flag"));
			ledges[i]->flags = flag;
			
			std::cout << "Adding new ledge: " << i << std::endl;
			
			for (tinyxml2::XMLElement* ee = e->FirstChildElement( "node" ); ee; ee = ee->NextSiblingElement() )
			{
				float locX = std::atof(ee->Attribute("x"));
				float locY = std::atof(ee->Attribute("y"));
				sf::Vector2<float> position(locX, locY);
				
				ledges[i]->nodes.push_back(position);
				
				std::cout << "Adding new node to ledge " << i << ": " << position.x << ", " << position.y << std::endl;
			}

			i++;
		}
	}
}

void Map::SaveMap(std::string filePath)
{
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
		tinyxml2::XMLElement* element = doc->NewElement( "segment" );
		element->SetAttribute( "id", mapSeg[i]->segmentIndex );
		element->SetAttribute( "x", mapSeg[i]->position.x );
		element->SetAttribute( "y", mapSeg[i]->position.y );
		element->SetAttribute( "layer", mapSeg[i]->layer );
		element->SetAttribute( "physics", mapSeg[i]->physicsObject );
		element->SetAttribute( "weight", mapSeg[i]->physicsWeight );
		element->SetAttribute( "rotation", mapSeg[i]->rotation );
		element->SetAttribute( "scalex", mapSeg[i]->scale.x );
		element->SetAttribute( "scaley", mapSeg[i]->scale.y );

		xemap->InsertEndChild(element);
	}

	tinyxml2::XMLElement* xeledges = doc->NewElement( "ledges" );
	tinyxml2::XMLNode* xnledges = doc->InsertEndChild(xeledges);

	for (int i = 0; i < ledges.size(); i++)
	{
		tinyxml2::XMLElement* xeledge = doc->NewElement( "ledge" );
		xeledge->SetAttribute( "flag", ledges[i]->flags );
		tinyxml2::XMLNode* xnledge = xeledges->InsertEndChild( xeledge );

		for (int n = 0; n < ledges[i]->nodes.size(); n++)
		{
			tinyxml2::XMLElement* element = doc->NewElement( "node" );
			element->SetAttribute( "x", ledges[i]->nodes[n].x );
			element->SetAttribute( "y", ledges[i]->nodes[n].y );
			xeledge->InsertEndChild(element);
		}
	}

	doc->SaveFile(a);
}