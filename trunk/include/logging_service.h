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

#ifndef __LOGGING_SERVICE_H__
#define __LOGGING_SERVICE_H__

#include "service.h"

#include <PA9.h>
#include <deque>
#include <string>

class LoggingService : public Service
{
public:
	LoggingService();
	virtual ~LoggingService();
	
	static LoggingService* getInstance();
	
	virtual void run();
	
	void logMessage(const std::string& msg);
	void setStaticText(u8 slot, const std::string& text);
	
private:
	static LoggingService* ms_instance;
	
	const std::string m_emptyLine;
	
	std::deque<std::string> m_log;
	std::string m_staticTexts[5];
	
	bool m_updateLog;
	bool m_updateStaticText;
};

#endif
