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

#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "game_state.h"

#include <stack>

class PlayGameState : public GameState
{
public:
	PlayGameState();
	virtual ~PlayGameState();
	
	virtual void resume();
	virtual void suspend();

	virtual u8 run();
	
	virtual u8 getId();
private:
	struct Place
	{
		u8 stoneId;
		u8 spriteId;
	};
	
	Place m_places[8*6];
	Place m_selection[3];
	
	std::stack<u8> m_availableSprites;
	
	bool m_isSuspending;
	
	void initPlayField();
	u8 getRandomStone();
	u8 createSprite(u8 stoneId, u8 x, u8 y);
	void destroySprite(u8 spriteId);
	void setSpriteSelected(u8 spriteId, bool selected);
	
	void showPlayfield();
	void hidePlayfield();
	
	bool isSelected(const Place& place) const;
	void setSelected(const Place& place);
	void setUnselected(const Place& place);
	u8 getNumSelected() const;
};

#endif
