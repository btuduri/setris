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

#include "highscore_game_state.h"

#include "game_loop.h"

HighscoreGameState::HighscoreGameState()
	: m_isSuspending(false)
{
}

HighscoreGameState::~HighscoreGameState()
{
}

void HighscoreGameState::resume()
{
	m_isSuspending = false;
		
	// Show highscore
	PA_SetTextCol(0, 31, 31, 31);
	PA_OutputSimpleText(0, 10, 10, "Highscores");
	PA_OutputSimpleText(0, 10, 11, "==========");
	PA_OutputSimpleText(0, 7, 14, "Haffax    100,000");
	PA_OutputSimpleText(0, 7, 16, "Haffax     10,000");
	PA_OutputSimpleText(0, 7, 18, "Haffax      1,000");
	
	// Show options.
	PA_OutputSimpleText(0, 24, 23, "B: Back");
}

void HighscoreGameState::suspend()
{
	PA_StopMod();
	
	// Clear screen
	/// @todo properly clear the screen, there likely is a function for this.
	PA_OutputSimpleText(0, 7, 10, "                    ");
	PA_OutputSimpleText(0, 7, 11, "                    ");
	PA_OutputSimpleText(0, 7, 14, "                    ");
	PA_OutputSimpleText(0, 7, 16, "                    ");
	PA_OutputSimpleText(0, 7, 18, "                    ");
	PA_OutputSimpleText(0, 24, 23, "       ");
	
	m_isSuspending = true;
}

u8 HighscoreGameState::run()
{
	if (m_isSuspending)
	{
		return STATE_DONE;
	}
	
	// handle input.
	// up / down to change options.
	// Button A to start currently selected option.
	
	// With only two options, this is easy.
	if (Pad.Newpress.B || Stylus.Newpress)
	{
		GameLoop::getInstance()->popState();
	}
		
	return STATE_RUNNING;
}

u8 HighscoreGameState::getId()
{
	return ID_HIGHSCORE;
}
