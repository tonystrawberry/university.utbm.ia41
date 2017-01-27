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

 
#include "BitVector.h"


#include "MemDefns.h"


BitVector::BitVector()
{
	m_Size = 0;
	m_Field = NULL;
}

BitVector::BitVector( int32 nbr_bits )
{
	m_NbrBits = nbr_bits;
	m_Size = ((nbr_bits+7)/8);
	m_Field = new uint8 [m_Size];
	memset( m_Field, 0, m_Size );
}

BitVector::~BitVector()
{
	SafeDeleteArray( m_Field );
}


void
BitVector::setSize( int32 nbr_bits )
{
	m_NbrBits = nbr_bits;
	m_Size = ((nbr_bits+7)/8);
	SafeDeleteArray( m_Field );
	m_Field = new uint8 [m_Size];
	memset( m_Field, 0, m_Size );
}


bool
BitVector::isSet( int32 i ) const
{
	return (m_Field[i/8] & (1 << (i%8)));
}


void
BitVector::reset()
{
	memset( m_Field, 0, m_Size );
}

void
BitVector::fill()
{
	memset( m_Field, 0xFF, m_Size );
}


void
BitVector::set( int32 i )
{
	m_Field[i/8] |= (1 << (i%8));
}

void
BitVector::unset( int32 i )
{
	m_Field[i/8] &= (0xFF ^ (1 << (i%8)));
}

void
BitVector::flip( int32 i )
{
	m_Field[i/8] ^= (1 << (i%8));
}


void
BitVector::display()
{
	for( int32 i = 0; i < m_NbrBits; ++i )
	{
		if (isSet( i ))
		{
			printf( "1" );
		}
		else
		{
			printf( "0" );
		}
	}
	printf( "\n" );
}
