/*
Copyright (c) 2008 Daniel Wickert

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "play_game_state.h"

#include "game_music.h"
#include "logging_service.h"

#include "stones_data.cpp"

#include <cstdio>
#include <cmath>

#define PI 3.14159265

PlayGameState::PlayGameState()
	: m_isSuspending(false)
{
	PA_LoadSpritePal(0, // Screen
		0, // Palette number
		(void*)stones_Pal);	// Palette name
		
	PA_SetRotset(0, 0, 0, 256, 256);
	PA_SetRotset(0, 1, 0, 300, 300);
		
	initPlayField();
	for (u8 i = 0; i < 128; ++i)
	{
		m_availableSprites.push(i);
	}
	
	for (u8 i = 0; i < 3; ++i)
	{
		m_selection[i].spriteId = 255;
	}
	
	// Prepare animation sizes
	u8 stepsize = 360 / 60;
	char msg[32];
	for (u8 i = 0; i < 60; ++i)
	{
		m_animSequence[i] = (u16) (255 + 64 + (std::sin((PI/180.0f)*stepsize*i) * 64.0f));
		std::sprintf(msg, "Anim-Pos: %i", m_animSequence[i]);
		LoggingService::getInstance()->logMessage(msg);
	}

}

PlayGameState::~PlayGameState()
{
}

void PlayGameState::resume()
{
	m_isSuspending = false;
	
	// Play menu music
	PA_PlayMod(game_music);
	
	showPlayfield();
}

void PlayGameState::suspend()
{
	hidePlayfield();
	
	PA_StopMod();
	
	m_isSuspending = true;
}

u8 PlayGameState::run()
{
	if (m_isSuspending)
	{
		return STATE_DONE;
	}
	
	if (Stylus.Newpress)
	{
		char msg[32];
		std::sprintf(msg, "Stylus-Pos: %i / %i", Stylus.X, Stylus.Y);
		LoggingService::getInstance()->logMessage(msg);
		u8 x = Stylus.X / 32;
		u8 y = Stylus.Y / 32;
		std::sprintf(msg, "Tile-Pos: %i / %i", x, y);
		LoggingService::getInstance()->logMessage(msg);
		
		if (m_places[y*8+x].spriteId != 255)
		{
			std::sprintf(msg, "Hit stone with sprite-id %i", m_places[y*8+x].spriteId);
			LoggingService::getInstance()->logMessage(msg);
			// Player hit a sprite with the stylus.
			// is the place already selected?
			if (isSelected(m_places[y*8+x]))
			{
				// Yep, already selected, so deselect now.
				setUnselected(m_places[y*8+x]);
			}
			else
			{
				// Nope, not yet selected, so select now.
				setSelected(m_places[y*8+x]);
			}
			
			// Do we have three pieces selected now?
			if (getNumSelected() == 3)
			{
				if (checkSet())
				{
				}
				else
				{
				}
			}
		}
		else
		{
			std::sprintf(msg, "No stone hit");
			LoggingService::getInstance()->logMessage(msg);
		}
	}
	
	animSelectedSprites();
	
	return STATE_RUNNING;
}

u8 PlayGameState::getId()
{
	return ID_PLAY;
}

void PlayGameState::initPlayField()
{
	// Clear top half.
	for (u8 y = 0; y < 3; ++y)
	{
		for (u8 x = 0; x < 8; ++x)
		{
			m_places[y*8+x].stoneId = 255;
			m_places[y*8+x].spriteId = 255;
		}
	}
	
	// Fill bottom half with random stones.
	for (u8 y = 3; y < 6; ++y)
	{
		for (u8 x = 0; x < 8; ++x)
		{
			m_places[y*8+x].stoneId = getRandomStone();
			m_places[y*8+x].spriteId = 255;
		}
	}
}

u8 PlayGameState::getRandomStone()
{
	return static_cast<u8>(PA_RandMax(80));
}

u8 PlayGameState::createSprite(u8 stoneId, u8 x, u8 y)
{
	u8 id = m_availableSprites.top();
	m_availableSprites.pop();
	PA_CreateSprite(0, // Screen
		id, // Sprite number
		(void*)(stones_Sprites + stoneId*32*32), // Sprite data
		OBJ_SIZE_32X32, // Sprite size
		1, // 256 color mode
		0, // Sprite palette number
		x*32, y*32); // X and Y position on the screen
	//PA_SetSpritePrio(0, id, 10);

	return id;
}

void PlayGameState::destroySprite(u8 spriteId)
{
	PA_DeleteSprite(0, spriteId);
	m_availableSprites.push(spriteId);
}

void PlayGameState::setSpriteSelected(u8 spriteId, bool selected)
{
	PA_SetSpriteRotEnable(0, spriteId, selected ? 1 : 0);
}

void PlayGameState::animSelectedSprites()
{
	u16 scale = m_animSequence[m_animIndex];
	PA_SetRotset(0, 1, 0, scale, scale);
	m_animIndex = m_animIndex >= 59 ? 0 : m_animIndex+1;
}

void PlayGameState::showPlayfield()
{
	for (u8 i = 0; i < 48; ++i)
	{
		if (m_places[i].stoneId != 255)
		{
			m_places[i].spriteId = createSprite(m_places[i].stoneId, i % 8, i / 8);
		}
	}
}

void PlayGameState::hidePlayfield()
{
	for (u8 i = 0; i < 48; ++i)
	{
		if (m_places[i].spriteId != 255)
		{
			destroySprite(m_places[i].spriteId);
		}
	}
}

bool PlayGameState::isSelected(const Place& place) const
{
	return m_selection[0].spriteId == place.spriteId ||
		   m_selection[1].spriteId == place.spriteId ||
		   m_selection[2].spriteId == place.spriteId;
}

void PlayGameState::setSelected(const Place& place)
{
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i].spriteId == 255)
		{
			m_selection[i].spriteId = place.spriteId;
			setSpriteSelected(place.spriteId, true);
			break;
		}
	}
}

void PlayGameState::setUnselected(const Place& place)
{
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i].spriteId == place.spriteId)
		{
			m_selection[i].spriteId = 255;
			setSpriteSelected(place.spriteId, false);
			break;
		}
	}
}

u8 PlayGameState::getNumSelected() const
{
	u8 rval = 0;
	
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i].spriteId == 255)
		{
			++rval;
		}
	}
	
	return rval;
}

bool PlayGameState::checkSet() const
{
	bool ok = true;
	for (u8 i = 0; i < 4; ++i)
	{
		if (!ok)
		{
			break;
		}
	}
	return ok;
}
