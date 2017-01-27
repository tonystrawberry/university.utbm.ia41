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


#include "MemDefns.h"


template <class T>
StaticArray<T>::StaticArray()
{
	m_Size = 0;
	m_Values = NULL;
}

template <class T>
StaticArray<T>::StaticArray( uint32 size )
{
	m_Size = size;
	m_Values = new T [size];
}

template <class T>
StaticArray<T>::StaticArray( const StaticArray<T> & sa )
{
	m_Size = sa.size;
	m_Values = new T [m_Size];

	for( uint32 i = 0; i < m_Size; ++i )
	{
		m_Values[i] = sa[i];
	}
}

template <class T>
StaticArray<T>::~StaticArray()
{
	SafeDeleteArray( m_Values );
}


template <class T>
void
StaticArray<T>::init( uint32 size )
{
	SafeDeleteArray( m_Values );
	m_Size = size;
	m_Values = new T [size];
}


template <class T>
T &
StaticArray<T>::operator[]( uint32 i )
{
	return m_Values[i];
}


template <class T>
void
StaticArray<T>::fill( const T & v )
{
	for( uint32 i = 0; i < m_Size; ++i )
	{
		m_Values[i] = v;
	}
}


template <class T>
bool
StaticArray<T>::any( const T & v ) const
{
	for( uint32 i = 0; i < m_Size; ++i )
	{
		if (m_Values[i] == v)
		{
			return true;
		}
	}

	return false;
}


template <class T>
bool
StaticArray<T>::all( const T & v ) const
{
	for( uint32 i = 0; i < m_Size; ++i )
	{
		if (m_Values[i] != v)
		{
			return false;
		}
	}

	return true;
}
