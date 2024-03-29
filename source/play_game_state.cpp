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
#include "game_loop.h"

#include "stones_data.cpp"

#include <cstdio>
#include <cmath>

#define PI 3.14159265

const PlayGameState::Place PlayGameState::EMPTY_PLACE = { PlayGameState::EMPTY, PlayGameState::EMPTY };

PlayGameState::PlayGameState()
	: m_isSuspending(false), m_gameOver(false)
{	
	PA_LoadSpritePal(0, // Screen
		0, // Palette number
		(void*)stones_Pal);	// Palette name
		
	PA_SetRotset(0, 0, 0, 256, 256);
	PA_SetRotset(0, 1, 0, 300, 300);
		
	for (u8 i = 0; i < NUM_STONES; ++i)
	{
		m_availableStones.push_back(i);
		m_availableSprites.push(i);
	}
	
	initPlayField();
	
	for (u8 i = 0; i < 3; ++i)
	{
		m_selection[i] = EMPTY_PLACE;
	}
	
	// Prepare animation sizes
	u8 stepsize = 360 / 60;
	for (u8 i = 0; i < 60; ++i)
	{
		m_animSequence[i] = (u16) (255 + 64 + (std::sin((PI/180.0f)*stepsize*i) * 64.0f));
	}
	
	// Score, time and multiplicators.
	m_score = 0;
	m_sets = 0;
	m_level = 1;
	m_framesSinceLastSet = 0;
	m_framesSinceLastNewStone = 0;
	m_framesTillNextStone = 300;
	m_framesTillSetCombo = 180;
	
	m_comboMultiplicator = 1;
	m_sets = 0;
	
	LoggingService::getInstance()->setStaticText(0, " Score: 0");
	LoggingService::getInstance()->setStaticText(1, " Level: 1");
	LoggingService::getInstance()->setStaticText(2, " Sets : 0");
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
	PA_OutputSimpleText(0, 11, 10, "          ");
	
	PA_StopMod();
	
	m_isSuspending = true;
}

u8 PlayGameState::run()
{
	if (m_isSuspending)
	{
		return STATE_DONE;
	}
	
	if (m_gameOver)
	{
		if (Stylus.Newpress || Pad.Newpress.A || Pad.Newpress.B)
		{
			GameLoop::getInstance()->popState();
		}
		
		return STATE_RUNNING;
	}
	else if (Stylus.Newpress)
	{
		char msg[32];
		std::sprintf(msg, "Stylus-Pos: %i / %i", Stylus.X, Stylus.Y);
		LoggingService::getInstance()->logMessage(msg);
		u8 x = Stylus.X / 32;
		u8 y = Stylus.Y / 32;
		std::sprintf(msg, "Tile-Pos: %i / %i", x, y);
		LoggingService::getInstance()->logMessage(msg);
		
		if (m_places[y*8+x] != EMPTY_PLACE)
		{
			std::sprintf(msg, "Hit sprite-id %i stone-id %i", m_places[y*8+x].spriteId, m_places[y*8+x].stoneId);
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
					LoggingService::getInstance()->logMessage("Set!!");
					// Remove stones and compact field
					destroySelected();
					compactPlayfield();
					// Calculate new score
					m_sets++;
					m_setsTillNextLevel++;
					m_score += m_level*m_comboMultiplicator;
					
					// Adjust multiplicators.
					if (m_framesSinceLastSet < m_framesTillSetCombo)
					{
						m_comboMultiplicator++;
						m_framesSinceLastSet = 0;
					}
					else
					{
						m_comboMultiplicator = 1;
					}
					
					// adjust level
					if (m_setsTillNextLevel >= 10)
					{
						m_setsTillNextLevel = 0;
						if (m_level < 10) m_level++;
						m_framesTillNextStone = 300 - (30*(m_level-1));
					}
					
					std::sprintf(msg, " Score: %i", m_score);
					LoggingService::getInstance()->setStaticText(0, msg);
					std::sprintf(msg, " Level: %i", m_level);
					LoggingService::getInstance()->setStaticText(1, msg);
					std::sprintf(msg, " Sets : %i", m_sets);
					LoggingService::getInstance()->setStaticText(2, msg);
				}
				else
				{
					LoggingService::getInstance()->logMessage("No Set, aaaw");
					// deselect all stones.
					unselectAll();
				}
			}
		}
		else
		{
			std::sprintf(msg, "No stone hit");
			LoggingService::getInstance()->logMessage(msg);
		}
	}
	
	if (m_framesSinceLastNewStone > m_framesTillNextStone)
	{
		addNewStone();
		m_framesSinceLastNewStone = 0;
	}
	
	animSelectedSprites();
	moveSprites();
	
	m_framesSinceLastSet++;
	m_framesSinceLastNewStone++;
	
	return STATE_RUNNING;
}

u8 PlayGameState::getId()
{
	return ID_PLAY;
}

void PlayGameState::initPlayField()
{
	// Clear top half.
	for (u8 y = 0; y < NUM_ROWS / 2; ++y)
	{
		for (u8 x = 0; x < NUM_COLS; ++x)
		{
			m_places[y*NUM_COLS+x] = EMPTY_PLACE;
		}
	}
	
	// Fill bottom half with random stones.
	for (u8 y = NUM_ROWS / 2; y < NUM_ROWS; ++y)
	{
		for (u8 x = 0; x < NUM_COLS; ++x)
		{
			m_places[y*NUM_COLS+x].stoneId = getRandomStone();
			m_places[y*NUM_COLS+x].spriteId = EMPTY;
		}
	}
}

u8 PlayGameState::getRandomStone()
{
	u8 idx =  static_cast<u8>(PA_RandMax(m_availableStones.size()-1));
	u8 rval = m_availableStones[idx];
	m_availableStones.erase(m_availableStones.begin() + idx);
	return rval;
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

	return id;
}

void PlayGameState::destroySprite(u8 spriteId)
{
	char msg[32];
	std::sprintf(msg, "desroy %i !!!", spriteId);
	LoggingService::getInstance()->logMessage(msg);
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
	for (u8 i = 0; i < NUM_PLACES; ++i)
	{
		if (m_places[i].stoneId != EMPTY)
		{
			m_places[i].spriteId = createSprite(m_places[i].stoneId, i % NUM_COLS, i / NUM_COLS);
		}
	}
}

void PlayGameState::hidePlayfield()
{
	for (u8 i = 0; i < NUM_PLACES; ++i)
	{
		if (m_places[i].spriteId != EMPTY)
		{
			destroySprite(m_places[i].spriteId);
			m_places[i].spriteId = EMPTY;
		}
	}
}

bool PlayGameState::isSelected(const Place place) const
{
	return m_selection[0].spriteId == place.spriteId ||
		   m_selection[1].spriteId == place.spriteId ||
		   m_selection[2].spriteId == place.spriteId;
}

void PlayGameState::setSelected(const Place place)
{
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i] == EMPTY_PLACE)
		{
			m_selection[i] = place;
			setSpriteSelected(place.spriteId, true);
			break;
		}
	}
}

void PlayGameState::unselectAll()
{
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i] != EMPTY_PLACE)
		{
			setSpriteSelected(m_selection[i].spriteId, false);
			m_selection[i] = EMPTY_PLACE;
		}
	}
}

void PlayGameState::destroySelected()
{
	for (u8 i = 0; i < 3; ++i)
	{
		Place p = m_selection[i];
		if (p != EMPTY_PLACE)
		{
			// find the place
			for (u8 ii = 0; ii < NUM_PLACES; ++ii)
			{
				if (p == m_places[ii])
				{
					setSpriteSelected(p.spriteId, false);
					m_availableStones.push_back(p.stoneId);
					destroySprite(p.spriteId);
					m_places[ii] = EMPTY_PLACE;
				}
			}
		}
		m_selection[i] = EMPTY_PLACE;
	}
}

void PlayGameState::setUnselected(const Place place)
{
	for (u8 i = 0; i < 3; ++i)
	{
		if (m_selection[i].spriteId == place.spriteId)
		{
			m_selection[i] = EMPTY_PLACE;
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
		if (m_selection[i] != EMPTY_PLACE)
		{
			++rval;
		}
	}
	
	return rval;
}

bool PlayGameState::checkSet() const
{
	bool ok = false;
	u8 factor = 1;
	for (u8 i = 0; i < 4; ++i)
	{
		ok = false;
		
		u8 v1 = (m_selection[0].stoneId / factor) % 3;
		u8 v2 = (m_selection[1].stoneId / factor) % 3;
		u8 v3 = (m_selection[2].stoneId / factor) % 3;
		
		if (v1 == v2 && v2 == v3 && v3 == v1)
		{
			ok = true;
		}
		if (v1 != v2 && v2 != v3 && v3 != v1)
		{
			ok = true;
		}
		if (!ok)
		{
			break;
		}
		factor *= 3;
	}
	return ok;
}

void PlayGameState::compactPlayfield()
{
	// Process each column from bottom to top.
	for (u8 x = 0; x < NUM_COLS; ++x)
	{
		u8 gap = 0; // y-index of the last gap found.
		for (u8 yy = NUM_ROWS; yy > 0; --yy)
		{
			u8 y = yy-1;
			if (m_places[y*8+x] == EMPTY_PLACE)
			{
				// Found a gap.
				if (gap < y) gap = y;
			}
			else if (gap > y)
			{
				// A stone over a gap. Compact it.
				m_places[gap*8+x] = m_places[y*8+x];
				m_places[y*8+x] = EMPTY_PLACE;
				FallingSprite fs;
				fs.spriteId = m_places[gap*8+x].spriteId;
				fs.targetPlace = gap*8+x;
				fs.velocity = 1;
				m_fallingSprites.push_back(fs);
				
				--gap;
			}
		}
	}
}

void PlayGameState::moveSprites()
{
	// Move sprites and increment velocity.
	for (u8 i = 0; i < m_fallingSprites.size();)
	{
		u8 targetY = (m_fallingSprites[i].targetPlace / NUM_COLS) * 32;
		u8 currentY = PA_GetSpriteY(0, m_fallingSprites[i].spriteId);
		currentY += m_fallingSprites[i].velocity;
		currentY = std::min(targetY, currentY);
		m_fallingSprites[i].velocity += 2;
		PA_SetSpriteY(0, m_fallingSprites[i].spriteId, currentY);
		if (currentY == targetY) m_fallingSprites.erase(m_fallingSprites.begin() + i);
		else ++i;
	}
}

void PlayGameState::addNewStone()
{
	u8 x = PA_RandMax(NUM_COLS - 1);
	if (m_places[x] == EMPTY_PLACE)
	{
			m_places[x].stoneId = getRandomStone();
			m_places[x].spriteId = createSprite(m_places[x].stoneId, x, 0);
			compactPlayfield();
	}
	else
	{
		// Game over
		gameOver();
	}
}

void PlayGameState::gameOver()
{
	m_gameOver = true;
	hidePlayfield();
	PA_StopMod();
	PA_OutputSimpleText(0, 11, 10, "Game Over!");
}
