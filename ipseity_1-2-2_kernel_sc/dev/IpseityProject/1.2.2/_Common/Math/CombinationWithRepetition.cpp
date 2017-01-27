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

 
#include "CombinationWithRepetition.h"


#include <iostream>

#include "Types.h"
#include "MemDefns.h"


using namespace std;


CombinationWithRepetition::CombinationWithRepetition() :
    Counts( NULL )
{
}

CombinationWithRepetition::CombinationWithRepetition( uint32 n, uint32 r ) :
    Counts( NULL )
{
    init( n, r );
}


CombinationWithRepetition::~CombinationWithRepetition()
{
    clear();
}


void
CombinationWithRepetition::init( uint32 n, uint32 r )
{
    clear();

    m_N = n;
    m_R = r;
    Counts = new uint32 [n];

    reset();
}


void
CombinationWithRepetition::reset()
{
    Counts[0] = m_R;
    for( uint32 i = 1; i < m_N; ++i )
    {
        Counts[i] = 0;
    }
}


bool
CombinationWithRepetition::next()
{
    uint32 i = m_N-1;

    if (Counts[i] != m_R)
    {
        uint32 tmp;

        tmp = Counts[i];
        Counts[i] = 0;

        while( Counts[i] == 0 )
        {
            --i;
        }
        --Counts[i];
        Counts[i+1] = tmp+1;

        return true;
    }
    else
    {
        return false;
    }
}


void
CombinationWithRepetition::consoleDisplay()
{
    uint32 i;

    for( i = 0; i < m_N-1; ++i )
    {
        cout << Counts[i] << ", ";
    }
    cout << Counts[i];
    cout << endl;
}


void
CombinationWithRepetition::clear()
{
    SafeDeleteArray( Counts );
}
