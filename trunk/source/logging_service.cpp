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

#include "logging_service.h"

LoggingService* LoggingService::ms_instance = NULL;

LoggingService* LoggingService::getInstance()
{
	return ms_instance;
}

LoggingService::LoggingService()
	: m_emptyLine(32, ' '),
	  m_log(),
	  m_staticTexts(),
	  m_updateLog(true),
	  m_updateStaticText(true)
{
	PA_InitText(1, 0);
	
	ms_instance = this;
	PA_OutputSimpleText(1, 0, 18, "================================");
	
	for (u8 i = 0; i < 5; ++i)
	{
		m_staticTexts[i] = "X";
	}
}

LoggingService::~LoggingService()
{
	ms_instance = NULL;
}

void LoggingService::run()
{
	if(!isActive())
	{
		return;
	}
	
	if (m_updateLog)
	{
		for (u8 i = 0; i < m_log.size(); ++i)
		{
			PA_OutputSimpleText(1, 0, i, m_emptyLine.c_str());
			PA_OutputSimpleText(1, 0, i, m_log[i].c_str());
		}
		m_updateLog = false;
	}
	
	if (m_updateStaticText)
	{
		for (u8 i = 0; i < 5; ++i)
		{
			PA_OutputSimpleText(1, 0, 19 + i, m_emptyLine.c_str());
			PA_OutputSimpleText(1, 0, 19 + i, m_staticTexts[i].c_str());
		}
		m_updateStaticText = false;
	}
}

void LoggingService::logMessage(const std::string& msg)
{
	m_log.push_back(msg.substr(0, 32));
	while (m_log.size() > 18) m_log.pop_front();
	m_updateLog = true;
}

void LoggingService::setStaticText(u8 slot, const std::string& text)
{
	if (slot > 4)
	{
		return;
	}
	m_staticTexts[slot] = text.substr(0, 32);
	m_updateStaticText = true;
}
