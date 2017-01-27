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


#ifndef __PROBABILITY_MASS__
#define __PROBABILITY_MASS__


#include "Types.h"


class ProbabilityMass
{
	public:
		ProbabilityMass();

		void generate( uint32 nbrProbabilities, float64 maxProbability = 1.0 );

		float64 & operator[]( uint32 i );

	protected:
		void reset();
		void withdraw();
		float64 get();


	protected:
		uint32 m_NbrProbabilities;
		float64* m_Probabilities;

		uint32 m_Nr;
		float64 m_CurrentMass;
		float64 m_WithdrawedProbability;
		float64 m_MaxProbabilityToWithdraw;
/*
		uint32 m_MaxUnits;
		uint32 m_CurrentUnits;
		uint32 m_WithdrawedUnits;
		uint32 m_MinUnitsToWithdraw;
		uint32 m_MaxUnitsToWithdraw;
		float64 m_ProbabilityUnit;
*/
};


#endif
