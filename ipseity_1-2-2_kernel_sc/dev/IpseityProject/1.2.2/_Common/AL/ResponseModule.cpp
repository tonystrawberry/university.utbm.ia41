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


#include "ResponseModule.h"

#include <iostream>


void
ResponseModule::setResponseSignalProperties( ResponseSignalProperties* rp )
{
    m_ResponseSignalProperties = rp;
}


const Response &
ResponseModule::selectFirstResponse( const SensorySignal & s )
{
    m_NextResponse.setToMinVector( m_ResponseSignalProperties );

	return m_NextResponse;
}


bool
ResponseModule::selectNextResponse( const SensorySignal & s, Response & a )
{
    bool r = m_NextResponse.inc( m_ResponseSignalProperties );
    a = m_NextResponse;

    return r;
}


const Response &
ResponseModule::selectRandomResponse( const SensorySignal & s )
{
    m_NextResponse.setRandomVector( m_ResponseSignalProperties );

    return m_NextResponse;
}
