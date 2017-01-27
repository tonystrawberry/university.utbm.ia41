/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "Timer.h"


Timer::Timer()
{
    PerfTime = QueryPerformanceFrequency( (LARGE_INTEGER*) &Frq );
	if (PerfTime)
    {
		QueryPerformanceCounter( (LARGE_INTEGER*) &Time1 );
		OneTick = 1.0/Frq;
    }
    else
    {
//		Time1 = timeGetTime();
		OneTick = 0.001;
    }

	m_ElapsedTime = 0.0;
	m_NbrEvents = 0;
}


float64
Timer::getElapsedSeconds()
{
	Time0 = Time1;
	if (PerfTime)
	{
		QueryPerformanceCounter( (LARGE_INTEGER*) &Time1 );
	}
    else
	{
//		Time1 = timeGetTime();
	}

	dt = (Time1-Time0) * OneTick;

	return dt;
}


bool
Timer::computeFrequency()
{
	m_ElapsedTime += getElapsedSeconds();
	m_NbrEvents++;

	if (m_ElapsedTime >= 1.0)
	{
		m_Frequency = m_NbrEvents;

		m_ElapsedTime = 0.0;
		m_NbrEvents = 0;

		return true;
	}
	else
	{
		return false;
	}
}


bool
Timer::getTimeTick()
{
	return (m_NbrEvents == 0);
}


uint32
Timer::getFrequency()
{
	return m_Frequency;
}
