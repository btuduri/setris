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

#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <PA9.h>

class GameState
{
public:
	static const u8 STATE_RESUMING = 1;
	static const u8 STATE_RUNNING = 2;
	static const u8 STATE_SUSPENDING = 3;
	static const u8 STATE_DONE = 4;
	
	GameState() {};
	virtual ~GameState() {};
	
	virtual void resume() = 0;
	virtual void suspend() = 0;

	/**
	 * Advances this state. Called every frame that this state is active.
	 * @return the internal state of this game state.
	 */
	virtual u8 run() = 0;
	
	virtual u8 getId() = 0;
};

#endif
