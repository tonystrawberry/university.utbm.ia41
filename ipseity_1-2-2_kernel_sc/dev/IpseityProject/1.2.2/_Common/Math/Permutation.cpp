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


#include "Permutation.h"


#include "MemDefns.h"
#include "Math/RandomNumberGenerator.h"

#include <string.h>


Permutation::Permutation( uint32 nbr_elements )
{
	m_NbrElements = nbr_elements;
	m_Done = new bool [nbr_elements];
	m_Elements = new uint32 [nbr_elements];

	RandomNumberGenerator::initSeed( 0 );
}

Permutation::~Permutation()
{
	SafeDeleteArray( m_Done );
	SafeDeleteArray( m_Elements );
}


uint32
Permutation::operator []( uint32 i ) const
{
	return m_Elements[i];
}


void
Permutation::shuffle()
{
	int32 nbr_elements = (int32) m_NbrElements;
	uint32 nr;

	memset( m_Done, 0, sizeof( bool )*m_NbrElements );

	for( uint32 i = 0; i < m_NbrElements; ++i )
	{
		do
		{
			nr = RandomNumberGenerator::getRandomNumber( nbr_elements );
		}
		while( m_Done[nr] );

		m_Done[nr] = true;
		m_Elements[i] = nr;
	}
}
