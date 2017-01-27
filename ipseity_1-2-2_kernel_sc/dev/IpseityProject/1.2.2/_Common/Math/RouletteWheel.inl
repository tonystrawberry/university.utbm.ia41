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

 
#include "Math/RandomNumberGenerator.h"

#include <iostream>


template <class T>
RouletteWheel<T>::RouletteWheel( uint32 vs, T sc, bool withRepetition )
{
	m_VectorSize = vs;

	m_SetCardinal = sc;
	if( withRepetition )
	{
		m_Flags = NULL;
	}
	else
	{
		m_Flags = new bool [sc];
	}
	m_Probabilities = NULL;

	m_Shift = (T) 0;
}

template <class T>
RouletteWheel<T>::RouletteWheel( uint32 vs, T* vector, T sc, bool withRepetition )
{
	m_VectorSize = vs;
	m_Vector = vector;

	m_SetCardinal = sc;
	if( withRepetition )
	{
		m_Flags = NULL;
	}
	else
	{
		m_Flags = new bool [sc];
	}
	m_Probabilities = NULL;

	m_Shift = (T) 0;
}


template <class T>
void RouletteWheel<T>::setVector( T* vector )
{
	m_Vector = vector;
}

template <class T>
void RouletteWheel<T>::setProbabilities( float64* probabilities )
{
	m_Probabilities = probabilities;
}

template <class T>
void RouletteWheel<T>::setShift( T shift)
{
	m_Shift = shift;
}


template <class T>
void RouletteWheel<T>::generate()
{
	uint32 i;
	T nr;

	if( m_Flags )
	{
		for( i = 0; i < m_SetCardinal; i++ )
		{
			m_Flags[i] = false;
		}
	}

	if( NULL == m_Probabilities )	// Using uniform probabilities
	{
		if( m_Flags )	// Without repetition
		{
			for( i = 0; i < m_VectorSize; i++ )
			{
				do
				{
					nr = RandomNumberGenerator::getRandomNumber( (int32) m_SetCardinal );
				}
				while( m_Flags[nr] );
				m_Flags[nr] = true;

				m_Vector[i] = m_Shift+nr;
			}
		}
		else			// With repetition
		{
			for( i = 0; i < m_VectorSize; i++ )
			{
				nr = RandomNumberGenerator::getRandomNumber( (int32) m_SetCardinal );

				m_Vector[i] = m_Shift+nr;
			}
		}
	}
	else							// Using probabilities
	{
		float64 r;
		float64 limit;

		if( m_Flags )	// Without repetition
		{
			for( i = 0; i < m_VectorSize; ++i )
			{
				do
				{
					r = RandomNumberGenerator::getRandomNumber( 1.0 );

					limit = 0.0;
					for( nr = 0; nr < m_SetCardinal; ++nr )
					{
						if( m_Probabilities[nr] > 0.0 )
						{
							limit += m_Probabilities[nr];

							if( r <= limit )
							{
								break;
							}
						}
					}
				}
				while( m_Flags[nr] );
				m_Flags[nr] = true;

				m_Vector[i] = m_Shift+nr;
			}
		}
		else
		{
			for( i = 0; i < m_VectorSize; ++i )
			{
				r = RandomNumberGenerator::getRandomNumber( 1.0 );

				limit = 0.0;
				for( nr = 0; nr < m_SetCardinal; ++nr )
				{
					if( m_Probabilities[nr] > 0.0 )
					{
						limit += m_Probabilities[nr];

						if( r <= limit )
						{
							break;
						}
					}
				}

				m_Vector[i] = m_Shift+nr;
			}
		}
	}
}


template <typename T>
std::ostream &
operator<<( const std::ostream & os, const RouletteWheel<T> & rw )
{
	for( uint32 i = 0; i < rw.m_VectorSize; i++ )
	{
		os << rw.m_Vector[i] << " ";
	}
	os << std::endl;

	return os;
}
