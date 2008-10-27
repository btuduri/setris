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

#include "game_loop.h"

#include "game_state.h"
#include "menu_game_state.h"
#include "play_game_state.h"
#include "highscore_game_state.h"
#include "pause_game_state.h"
#include "logging_service.h"

GameLoop* GameLoop::ms_instance = NULL;

GameLoop* GameLoop::getInstance()
{
	return ms_instance;
}

GameLoop::GameLoop() : m_states(), m_services(), m_nextState(NULL)
{
	PA_Init();
	PA_InitVBL();
	PA_InitRand();
	
	AS_Init(AS_MODE_16CH);
	
	// Init screen 0 to use text with font stored in bg0.
	PA_InitText(0, 0);
	
	GameState* state = createState(GameState::ID_MENU);
	m_states.push(state);
	
	// init services
	m_services.push_back(new LoggingService());
	
	ms_instance = this;
}

GameLoop::~GameLoop()
{
	// delete whatever state is still on the stack.
	while (!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
	
	ms_instance = NULL;
}

void GameLoop::run()
{
	m_states.top()->resume();
	while (1)
	{
		// run current state and check if state is done.
		if (m_states.top()->run() == GameState::STATE_DONE)
		{
			// Current state is done. Now either there is a new
			// state ready or not.
			if (m_nextState != NULL)
			{
				// new state has been pushed, now actually do it.
				m_states.push(m_nextState);
				m_states.top()->resume();
				m_nextState = NULL;
			}
			else
			{
				// destroy current top and resume state below on the stack.
				delete m_states.top();
				m_states.pop();
				m_states.top()->resume();
			}
		}
		
		doInnerLoop();
	}

}

void GameLoop::doInnerLoop()
{
	for (u8 i = 0; i < m_services.size(); ++i)
	{
		m_services[i]->run();
	}
	
	PA_WaitForVBL();
}

void GameLoop::pushState(u8 stateId)
{
	m_nextState = createState(stateId);
	m_states.top()->suspend();	
}

void GameLoop::popState()
{
	m_states.top()->suspend();	
}

void GameLoop::setState(u8 stateId)
{
	// empty stack.
	while (!m_states.empty())
	{
		m_states.top()->suspend();
		while (m_states.top()->run() == GameState::STATE_DONE)
		{
			doInnerLoop();
		}
		delete m_states.top();
		m_states.pop();
	}
	
	// push new one.
	m_states.push(createState(stateId));
	m_states.top()->resume();
}

GameState* GameLoop::createState(u8 stateId)
{
	/// @todo use a proper factory instead of hard coding states here.
	
	// Create the state instance depending on ID given.
	GameState* rval = NULL;
	if (stateId == GameState::ID_MENU)
	{
		rval = new MenuGameState();
	}
	else if (stateId == GameState::ID_PLAY)
	{
		rval = new PlayGameState();
	}
	else if (stateId == GameState::ID_HIGHSCORE)
	{
		rval = new HighscoreGameState();
	}
	return rval;
}
