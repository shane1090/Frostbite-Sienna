#include "stdafx.h"
#include "CharDef.h"

CharDef::CharDef(void)
{
	headIndex = 0;
	torsoIndex = 0;
	legsIndex = 0;
	weaponIndex = 0;
}


CharDef::~CharDef(void)
{

}

void CharDef::LoadCharacter(std::string filePath)
{
	char *a=new char[filePath.size()+1];
	a[filePath.size()]=0;
	memcpy(a,filePath.c_str(),filePath.size());
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a);

	tinyxml2::XMLElement* xeCharacter = doc.FirstChildElement( "character" );

	tinyxml2::XMLElement* xeFrames = xeCharacter->FirstChildElement( "frames" );

	if (xeFrames != NULL)
	{
		int i = 0;
		for (tinyxml2::XMLElement* e = xeFrames->FirstChildElement( "frame" ); e; e = e->NextSiblingElement() )
		{
			frames.push_back(new Frame);
			
			std::string c = e->Attribute("name");
			frames[i]->name = c;
			
			std::cout << "Adding new frame: " << i << std::endl;

			tinyxml2::XMLElement* xeParts = e->FirstChildElement( "parts" );
			
			int j = 0;
			for (tinyxml2::XMLElement* ee = xeParts->FirstChildElement( "part" ); ee; ee = ee->NextSiblingElement() )
			{
				sf::Vector2<float> location(std::atof(ee->Attribute("x")), std::atof(ee->Attribute("y")));
				sf::Vector2<float> scale(std::atof(ee->Attribute("scalex")), std::atof(ee->Attribute("scaley")));
				float rotation = std::atof(ee->Attribute("rotation"));
				int index = std::atoi(ee->Attribute("index"));
				int flip = std::atoi(ee->Attribute("flip"));

				frames[i]->parts.push_back(new Part(index));
				frames[i]->parts[j]->location = location;
				frames[i]->parts[j]->scaling = scale;
				frames[i]->parts[j]->rotation = rotation;
				frames[i]->parts[j]->flip = flip;

				std::cout << "Adding new part to frame " << i << ": " << std::endl;

				j++;
			}

			i++;
		}
	}

	tinyxml2::XMLElement* xeAnimations = xeCharacter->FirstChildElement( "animations" );

	if (xeAnimations != NULL)
	{
		int i = 0;
		for (tinyxml2::XMLElement* e = xeAnimations->FirstChildElement( "animation" ); e; e = e->NextSiblingElement() )
		{
			animations.push_back(new Animation);
			
			std::string c = e->Attribute("name");
			animations[i]->name = c;
			
			std::cout << "Adding new animation: " << i << std::endl;

			tinyxml2::XMLElement* xeKeyFrames = e->FirstChildElement( "keyframes" );
			
			if (xeAnimations != NULL)
			{
				for (tinyxml2::XMLElement* ee = xeKeyFrames->FirstChildElement( "frame" ); ee; ee = ee->NextSiblingElement() )
				{
					int frameRef = std::atoi(ee->Attribute("frameRef"));
					int duration = std::atoi(ee->Attribute("duration"));
				
					animations[i]->keyFrames.push_back(new KeyFrame(frameRef, duration));
				
					std::cout << "Adding new key frame to animation " << i << ": " << std::endl;
				}
			}

			i++;
		}
	}

}

void CharDef::SaveCharacter(std::string filePath)
{
	char *a=new char[filePath.size()+1];
	a[filePath.size()]=0;
	memcpy(a,filePath.c_str(),filePath.size());
	
	// XML file creation
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLDeclaration* xdcharacter = doc->NewDeclaration();
	doc->InsertEndChild(xdcharacter);
	
	tinyxml2::XMLElement* xeCharacter = doc->NewElement( "character" );
	tinyxml2::XMLNode* xnCharacter = doc->InsertEndChild(xeCharacter);

	tinyxml2::XMLElement* xeAnimations = doc->NewElement( "animations" );
	tinyxml2::XMLNode* xnAnimations = xnCharacter->InsertEndChild(xeAnimations);
	
	for (int i = 0; i < animations.size(); i++)
	{
		tinyxml2::XMLElement* xeAnimation = doc->NewElement( "animation" );
		const char * c = animations[i]->name.c_str();
		xeAnimation->SetAttribute( "name", c );
		tinyxml2::XMLNode* xnAnimation = xnAnimations->InsertEndChild( xeAnimation );

		tinyxml2::XMLElement* xeKeyFrames = doc->NewElement( "keyframes" );
		tinyxml2::XMLNode* xnKeyFrames = xnAnimation->InsertEndChild(xeKeyFrames);

		for (int j = 0; j < animations[i]->keyFrames.size(); j++)
		{
			tinyxml2::XMLElement* element = doc->NewElement( "frame" );
			element->SetAttribute( "frameRef", animations[i]->keyFrames[j]->frameRef );
			element->SetAttribute( "duration", animations[i]->keyFrames[j]->duration );
			xnKeyFrames->InsertEndChild(element);
		}
	}

	tinyxml2::XMLElement* xeFrames = doc->NewElement( "frames" );
	tinyxml2::XMLNode* xnFrames = xnCharacter->InsertEndChild(xeFrames);

	for (int i = 0; i < frames.size(); i++)
	{
		tinyxml2::XMLElement* xeFrame = doc->NewElement( "frame" );
		const char * c = frames[i]->name.c_str();
		xeFrame->SetAttribute( "name", c );
		tinyxml2::XMLNode* xnFrame = xnFrames->InsertEndChild( xeFrame );

		tinyxml2::XMLElement* xeParts = doc->NewElement( "parts" );
		tinyxml2::XMLNode* xnParts = xnFrame->InsertEndChild(xeParts);

		for (int j = 0; j < frames[i]->parts.size(); j++)
		{
			tinyxml2::XMLElement* element = doc->NewElement( "part" );
			element->SetAttribute( "x", frames[i]->parts[j]->location.x );
			element->SetAttribute( "y", frames[i]->parts[j]->location.y );
			element->SetAttribute( "scalex", frames[i]->parts[j]->scaling.x );
			element->SetAttribute( "scaley", frames[i]->parts[j]->scaling.y );
			element->SetAttribute( "rotation", frames[i]->parts[j]->rotation );
			element->SetAttribute( "index", frames[i]->parts[j]->index );
			element->SetAttribute( "flip", frames[i]->parts[j]->flip );
			xnParts->InsertEndChild(element);
		}
	}

	doc->SaveFile(a);
}
