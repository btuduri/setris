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
#include <vector>

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
	
	struct FallingSprite
	{
		u8 spriteId;
		u8 targetPlace;
		u8 velocity;
	};
	
	Place m_emptyPlace;
	
	Place m_places[8*6];
	Place m_selection[3];
	
	u16 m_animSequence[60];
	u8 m_animIndex;
	
	int m_score;
	
	u16 m_framesSinceLastSet;
	u16 m_framesSinceLastNewStone;
	
	u16 m_framesTillNextStone;
	u16 m_framesTillSetCombo;
	
	u16 m_comboMultiplicator;
	u8 m_sets;
	
	std::stack<u8> m_availableSprites;
	std::vector<u8> m_availableStones;
	std::vector<FallingSprite> m_fallingSprites;
	
	bool m_isSuspending;
	
	void initPlayField();
	
	u8 getRandomStone();
	
	u8 createSprite(u8 stoneId, u8 x, u8 y);
	void destroySprite(u8 spriteId);
	void setSpriteSelected(u8 spriteId, bool selected);
	void animSelectedSprites();
	
	void showPlayfield();
	void hidePlayfield();
	
	bool isSelected(const Place& place) const;
	void setSelected(const Place& place);
	void setUnselected(const Place& place);
	void unselectAll();
	/// Selected stones are removed from the playfield.
	void destroySelected();
	/// Reorganise playfield to fill gaps.
	void compactPlayfield();
	/// let falling sprites fall.
	void moveSprites();
	
	void addNewStone();
	
	
	u8 getNumSelected() const;
	
	bool checkSet() const;
};

#endif