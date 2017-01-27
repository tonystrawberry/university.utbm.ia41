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


#ifndef RouletteWheel_h
#define RouletteWheel_h


#include "Types.h"
#include <ostream>


template <class T>
class RouletteWheel
{
	public:
		RouletteWheel( uint32 vectorSize, T setCardinal, bool withRepetition = true );
		RouletteWheel( uint32 vectorSize, T* vector, T setCardinal, bool withRepetition = true );

		void setVector( T* vector );
		void setProbabilities( float64* probabilities );
		void setShift( T );

		void generate();

//		template <typename T>
//		friend std::ostream & operator<<( const std::ostream & os, const RouletteWheel<T> & );


	protected:
		uint32 m_VectorSize;
		T* m_Vector;
		float64* m_Probabilities;
		T m_Shift;

		T m_SetCardinal;
		bool* m_Flags;
};


#include "RouletteWheel.inl"


#endif
