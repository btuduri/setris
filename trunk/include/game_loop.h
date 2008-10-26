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

#ifndef __GAME_LOOP_H__
#define __GAME_LOOP_H__

#include <PA9.h>
#include <stack>

class GameState;

class GameLoop
{
public:
	GameLoop();
	~GameLoop();
	
	void run();
	
	/// Suspends current state and starts the given state.
	void pushState(u8 stateId);
	/// Suspends and destroys current state and resumes the one below
	void popState();
	/// This will destroy all states on the stack and start given state.
	void setState(u8 stateId);
private:
	std::stack<GameState*> m_states;
	/// If not NULL, then this will be pushed on the stack, after current
	/// state is done.
	GameState* m_nextState;
	
	GameState* createState(u8 stateId);
};

#endif
