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

 
#include "CombinationWR.h"


#include <iostream>

#include "Types.h"
#include "MemDefns.h"


using namespace std;


CombinationWithoutRepetition::CombinationWithoutRepetition()
{
	Values = NULL;
}

CombinationWithoutRepetition::CombinationWithoutRepetition( uint32 n, uint32 r )
{
	Values = NULL;
	
	init( n, r );
}


CombinationWithoutRepetition::~CombinationWithoutRepetition()
{
	clear();
}


void CombinationWithoutRepetition::init( uint32 n, uint32 r )
{
	clear();

	m_N = n;
	m_R = r;
	m_Shift = m_N-m_R;
	Values = new uint32 [r];

	reset();
}


void CombinationWithoutRepetition::reset()
{
	for( uint32 i = 0; i < m_R; i++ )
	{
		Values[i] = i;
	}
}


bool CombinationWithoutRepetition::next()
{
	for( int m = m_R-1; m >= 0; m-- )
	{
		if( Values[m] != (m_Shift+m) )
		{
			Values[m]++;

			for( uint32 i = m+1; i < m_R; i++ )
			{
				Values[i] = Values[i-1]+1;
			}

			return true;
		}
	}

	return false;
}


void CombinationWithoutRepetition::consoleDisplay()
{
	uint32 i;

	for( i = 0; i < m_R-1; i++ )
	{
		cout << Values[i] << ", ";
	}
	cout << Values[i];
	cout << endl;
}


void CombinationWithoutRepetition::load( FILE * fp )
{
	uint32 n, r;

	fread( &n, sizeof( uint32 ), 1, fp );
	fread( &r, sizeof( uint32 ), 1, fp );

	Init( n, r );

	for( uint32 i = 0; i < m_R; i++ )
	{
		fread( &Values[i], sizeof( uint32 ), 1, fp );
	}
}

void CombinationWithoutRepetition::save( FILE * fp )
{
	fwrite( &_N, sizeof( uint32 ), 1, fp );
	fwrite( &_R, sizeof( uint32 ), 1, fp );

	for( uint32 i = 0; i < m_R; i++ )
	{
		fwrite( &Values[i], sizeof( uint32 ), 1, fp );
	}
}


void CombinationWithoutRepetition::clear()
{
	SafeDeleteArray( Values );
}
