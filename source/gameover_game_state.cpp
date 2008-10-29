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

#include "gameover_game_state.h"
#include "game_loop.h"

GameoverGameState::GameoverGameState() : m_isSuspending(false)
{
}

GameoverGameState::~GameoverGameState()
{
}

void GameoverGameState::resume()
{
	PA_OutputSimpleText(0, 12, 10, "Game Over");
}

void GameoverGameState::suspend()
{
	PA_OutputSimpleText(0, 12, 10, "         ");
	m_isSuspending = true;
}

u8 GameoverGameState::run()
{
	if (m_isSuspending)
	{
		return STATE_DONE;
	}

	if (Stylus.Newpress || Pad.Newpress.A || Pad.Newpress.B)
	{
		GameLoop::getInstance()->setState(ID_MENU);
	}
	return STATE_RUNNING;
}

u8 GameoverGameState::getId()
{
	return ID_GAMEOVER;
}
