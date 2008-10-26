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

#ifndef __STONE_MANAGER_H__
#define __STONE_MANAGER_H__

#define STONE_RED 0
#define STONE_BLUE 1
#define STONE_GREEN 2

#define STONE_THREE 0
#define STONE_TWO 1
#define STONE_ONE 2

#define STONE_RECTANGLE 0
#define STONE_ELLIPSE 1
#define STONE_WIGGLE 2

#define STONE_HALFFILLED 0
#define STONE_FILLED 1
#define STONE_UNFILLED 2


class Stone
{
public:
	Stone(u8 stoneId, u8 spriteId);
	u8 getStoneId() const;
	u8 getSpriteId() const;
	
	/// Returns true, if this and the other stone are equal in all attributes.
	bool matchesStone(const Stone& otherStone);
	/// Returns true, if this and the other stone differ in all attributes.
	bool differsFromStone(const Stone& otherStone);
	
private:
	const u8 m_stoneId;
	const u8 m_spriteId;
};

class StoneManager
{
public:
	static const u8 BOTTOM_SCREEN = 0;
	static const u8 TOP_SCREEN = 1;
	
	static const u8 MODE_HORIZONTAL = 0;
	static const u8 MODE_VERTICAL = 1;
	
	StoneManager(u8 screen = BOTTOM_SCREEN, u8 mode = MODE_HORIZONTAL);
	
    Stone* createStone(u8 stoneId);
	void destroyStone(Stone);
	
	bool isSet
	
private:
	u8 m_screen;
	u8 m_mode;
};

#endif
