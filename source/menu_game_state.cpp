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

#include "menu_game_state.h"

#include "menu_music.h"

MenuGameState::MenuGameState()
	: m_selectedOption(START_GAME_OPTION), m_isSuspending(false)
{
}

MenuGameState::~MenuGameState()
{
}

void MenuGameState::resume()
{
	m_isSuspending = false;
	
	// Play menu music
	PA_PlayMod(menu_music);
	
	// Show menu items
	PA_SetTextCol(0, 31, 31, 31);
	PA_OutputSimpleText(0, 12, 10, "Play");
	PA_OutputSimpleText(0, 12, 12, "Highscore");
	
	// Preselect play
	m_selectedOption = START_GAME_OPTION;
	PA_OutputSimpleText(0, 9, 10, "->");
}

void MenuGameState::suspend()
{
	PA_StopMod();
	
	m_isSuspending = true;
}

u8 MenuGameState::run()
{
	// handle input.
	// up / down to change options.
	// Button A to start currently selected option.
	
	// With only two options, this is easy.
	if (Pad.Newpress.Down || Pad.Newpress.Up)
	{
		m_selectedOption = (m_selectedOption + 1) % 2;
	}
	
	// Stylus control is possible too.
	bool stylusHit = false;
	if (Stylus.Newpress)
	{
		// First check X to see whether menu row has been hit.
		if (Stylus.X >= 9*8 && Stylus.X < 22*8)
		{
			// Now check Y to see what option has been hit, if any.
			if (Stylus.Y >= 10*8 && Stylus.Y < 11*8)
			{
				m_selectedOption = START_GAME_OPTION;
				stylusHit = true;
			}
			else if (Stylus.Y >= 12*8 && Stylus.Y < 13*8)
			{
				m_selectedOption = SHOW_HIGHSCORE_OPTION;
				stylusHit = true;
			}
		}
	}
	
	// Update selection marker
	PA_OutputSimpleText(0, 9, 10,
		m_selectedOption == START_GAME_OPTION ? "->" : "  ");
	PA_OutputSimpleText(0, 9, 12,
		m_selectedOption == SHOW_HIGHSCORE_OPTION ? "->" : "  ");
	
	if (stylusHit || Pad.Newpress.A)
	{
		// Start selected game state.
	}
	
	return m_isSuspending ? STATE_DONE : STATE_RUNNING;
}

u8 MenuGameState::getId()
{
	return 1;
}
