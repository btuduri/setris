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

#include "music.h"

MenuGameState::MenuGameState() : m_selectedOption(START_GAME_OPTION)
{
}

MenuGameState::~MenuGameState()
{
}
	
void MenuGameState::resume()
{
	PA_PlayMod(music);
}

void MenuGameState::suspend()
{
}

u8 MenuGameState::run()
{
	// handle input.
	// up / down to change options.
	// Button A to start currently selected option.
	
	// Stylus control is possible too.
	// Start item pointed at.
	
	return STATE_RUNNING;
}

u8 MenuGameState::getId()
{
	return 1;
}
