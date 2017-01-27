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


#include "ProbabilityMass.h"

#include "MemDefns.h"
#include "Math/RandomNumberGenerator.h"
#include "Math/MathToolBox_Misc.h"

#include <iostream>

using namespace std;


ProbabilityMass::ProbabilityMass() // uint32 nbrTotalUnits )
{
	m_Probabilities = NULL;
//	m_MaxUnits = nbrTotalUnits;
}


float64 & ProbabilityMass::operator[]( uint32 i )
{
	return m_Probabilities[i];
}


void ProbabilityMass::generate( uint32 nbrProbabilities, float64 maxProbability )
{
	m_NbrProbabilities = nbrProbabilities;
	SafeDeleteArray( m_Probabilities );
	m_Probabilities = new float64 [nbrProbabilities];

	reset();
/*
	uint32 maxUnits;

	maxUnits = (uint32) ceil( (maxPercentage / m_ProbabilityUnit) );

	m_MinUnitsToWithdraw = 0;
	if( maxUnits <= m_MinUnitsToWithdraw )
	{
		m_MaxUnitsToWithdraw = m_MaxUnits;
	}
	else
	{
		m_MaxUnitsToWithdraw = maxUnits;
	}
*/
	m_MaxProbabilityToWithdraw = maxProbability;

	for( uint32 i = 0; i < nbrProbabilities; ++i )
	{
		withdraw();
		m_Probabilities[i] = get();
	}
}


void ProbabilityMass::reset()
{
/*
	if( m_MaxUnits == 0 )
	{
		m_MaxUnits = m_NbrProbabilities*5000;
	}
*/
	m_Nr = m_NbrProbabilities;
	m_CurrentMass = 1.0;
/*
	m_CurrentUnits = m_MaxUnits;
	m_WithdrawedUnits = 0;
	m_ProbabilityUnit = 1.0 / m_MaxUnits;
*/
}

void ProbabilityMass::withdraw()
{
	if( m_Nr == 1 )
	{
		m_WithdrawedProbability = m_CurrentMass;
		m_CurrentMass = 0.0;

//		m_WithdrawedUnits = m_CurrentUnits;
//		m_CurrentUnits = 0;
	}
	else if( m_CurrentMass > 0.0 ) //m_CurrentUnits > 0 )
	{
/*
		uint32 max = MIN( (m_CurrentUnits+1), m_MaxUnitsToWithdraw );

		m_WithdrawedUnits = RandomGenerator::getRandomNumber( (int32) m_MinUnitsToWithdraw, (int32) max );
*/

		float64 max = MIN( m_CurrentMass, m_MaxProbabilityToWithdraw );
		m_WithdrawedProbability = RandomNumberGenerator::getRandomNumber( max );

		if( m_WithdrawedProbability > m_CurrentMass )
		{
			m_WithdrawedProbability = m_CurrentMass;
			m_CurrentMass = 0.0;
		}
		else
		{
			m_CurrentMass -= m_WithdrawedProbability;
		}
/*
		if( m_WithdrawedUnits > m_CurrentUnits )
		{
			m_WithdrawedUnits = m_CurrentUnits;
			m_CurrentUnits = 0;
		}
		else
		{
			m_CurrentUnits -= m_WithdrawedUnits;
		}
*/
		--m_Nr;
	}
	else
	{
		m_WithdrawedProbability = 0.0;

//		m_WithdrawedUnits = 0;

		--m_Nr;
	}

	cout << m_NbrProbabilities-m_Nr << ":" << m_WithdrawedProbability << "/" << m_CurrentMass << endl;
//	cout << m_NbrProbabilities-m_Nr << ":" << m_WithdrawedUnits << "/" << m_CurrentUnits << endl;
}


float64 ProbabilityMass::get()
{
	return m_WithdrawedProbability; //m_ProbabilityUnit*m_WithdrawedUnits;
}
