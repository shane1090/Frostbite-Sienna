#include "stdafx.h"
#include "Character.h"

Character::Character(void)
{
	location = sf::Vector2<float>(0.0f, 200.0f);
	trajectory = sf::Vector2<float>(0.0f, 0.0f);
	state = AIR;
	speed = 200.0f;
	ledgeAttach = -1;
}

Character::~Character(void)
{

}

void Character::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	sf::Time elapsed = gameTime.getElapsedTime();
	gameTime.restart();

	// Update location by trajectory
	sf::Vector2<float> pLocation = sf::Vector2<float>(location.x, location.y);

	if (state == GROUNDED)
	{
		if (trajectory.x > 0.0f)
		{
			trajectory.x -= FRICTION * elapsed.asSeconds();
			if (trajectory.x < 0.0f) trajectory.x = 0.0f;
		}

		if (trajectory.x < 0.0f)
		{
			trajectory.x += FRICTION * elapsed.asSeconds();
			if (trajectory.x > 0.0f) trajectory.x = 0.0f;
		}
	}

	location.x += trajectory.x * (float)elapsed.asSeconds();

	if (state == AIR)
	{
		location.y += trajectory.y * elapsed.asSeconds();
		trajectory.y += elapsed.asSeconds() * GRAVITY;
	}

	// Collision
	// Air Collision
	if (state == AIR)
	{
		if (trajectory.y > 0.0f)
		{
			for (int i = 0; i < map->ledges.size(); i++)
			{
				if (map->ledges[i]->nodes.size() > 1)
				{
					int ts = map->GetLedgeSec(i, pLocation.x);
					int s = map->GetLedgeSec(i, location.x);

					float fY = 0, tfY;

					if (s > -1 && ts > -1)
					{
						tfY = map->GetLedgeYLoc(i, s, pLocation.x);
						fY = map->GetLedgeYLoc(i, s, location.x);

						if (pLocation.y <= tfY && location.y >= fY)
						{
							location.y = fY;
							ledgeAttach = i;
							Land();
						}
						else
						{
							if (map->ledges[i]->flags == SOLID && location.y > fY)
							{
								location.y = fY;
								ledgeAttach = i;
								Land();
							}
						}
					}
				}
			}
		}
	}

	// Ground Collision
	else if (state == GROUNDED)
	{
		if (ledgeAttach > -1)
		{
			if (map->GetLedgeSec(ledgeAttach, location.x) == -1)
			{
				//FallOff();
			}
			else
			{
				location.y = map->GetLedgeYLoc(ledgeAttach, map->GetLedgeSec(ledgeAttach, location.x), location.x);
			}
		}
		else
		{
			// Check collision
		}
	}

	// Input
	if (InputManager::instance().HeldDown(sf::Keyboard::Left))
	{
		trajectory.x = -200.0f;
	}
	else if (InputManager::instance().HeldDown(sf::Keyboard::Right))
	{
		trajectory.x = 200.0f;
	}

	if (InputManager::instance().Pressed(sf::Keyboard::Space))
	{
		trajectory.y = -600.0f;
		state = AIR;
		ledgeAttach = -1;
	}
}

void Character::Draw(sf::RenderWindow &Window, sf::Vector2<float> &scroll)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(location.x - scroll.x, location.y - scroll.y);
	segmentShape.setSize(sf::Vector2<float>(20,40));
	segmentShape.setFillColor(sf::Color::Red);
	Window.draw(segmentShape);
}

void Character::Land()
{
	state = GROUNDED;
}