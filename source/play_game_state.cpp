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
#include "stones_data.cpp"

PlayGameState::PlayGameState()
	: m_isSuspending(false)
{
	PA_LoadSpritePal(0, // Screen
		0, // Palette number
		(void*)stones_Pal);	// Palette name
		
	initPlayField();
	for (u8 i = 0; i < 128; ++i)
	{
		m_availableSprites.push(i);
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
	return m_isSuspending ? STATE_DONE : STATE_RUNNING;
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

	return id;
}

void PlayGameState::destroySprite(u8 spriteId)
{
	PA_DeleteSprite(0, spriteId);
	m_availableSprites.push(spriteId);
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
