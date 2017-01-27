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


#include "Supervisor.h"

#include "Math/RandomNumberGenerator.h"

#include <iostream>

using namespace std;


Supervisor::Supervisor()
{
	m_Scenarii = NULL;
	m_PerformanceStats = NULL;

	m_Enabled = false;

	m_Ready = false;
	m_MinNbrEpisodes = 0;
	m_MinNbrEpisodesPerScenario = 0;
	m_MinSuccessPercentage = 0.9;
	m_MinSuccessPercentagePerScenario = 0.9;
}


void
Supervisor::setScenarii( Scenarii* scenarii )
{
	m_Scenarii = scenarii;
}

void
Supervisor::setPerformanceStats( PerformanceStats* ps )
{
	m_PerformanceStats = ps;
}


bool
Supervisor::isEnabled() const
{
	return m_Enabled;
}

void
Supervisor::setEnabled( bool e )
{
	m_Enabled = e;
}


bool
Supervisor::isReady() const
{
	return m_Ready;
}

void
Supervisor::setReady( bool ready )
{
	m_Ready = ready;
}


uint32
Supervisor::getMinNbrEpisodes() const
{
	return m_MinNbrEpisodes;
}

void
Supervisor::setMinNbrEpisodes( uint32 nbr )
{
	m_MinNbrEpisodes = nbr;
	cout << "# me " << nbr << endl;
}


uint32
Supervisor::getMinNbrEpisodesPerScenario() const
{
	return m_MinNbrEpisodesPerScenario;
}

void
Supervisor::setMinNbrEpisodesPerScenario( uint32 nbr )
{
	m_MinNbrEpisodesPerScenario = nbr;
	cout << "# meps " << nbr << endl;
}


float64
Supervisor::getMinSuccessPercentage() const
{
	return m_MinSuccessPercentage;
}

void
Supervisor::setMinSuccessPercentage( float64 perc )
{
	m_MinSuccessPercentage = perc;
	cout << "# sp " << perc << endl;
}


float64
Supervisor::getMinSuccessPercentagePerScenario() const
{
	return m_MinSuccessPercentagePerScenario;
}

void
Supervisor::setMinSuccessPercentagePerScenario( float64 perc )
{
	m_MinSuccessPercentagePerScenario = perc;
	cout << "# spps " << perc << endl;
}


uint32
Supervisor::selectScenario() const
{
	if (m_Scenarii)
	{
		uint32 index = m_Scenarii->getSize();

		return RandomNumberGenerator::getRandomNumber( (int32) index );
	}
	else
	{
		return 0;
	}
}


bool
Supervisor::isPerformanceReached() const
{
	if (m_PerformanceStats)
	{
		bool ret;

		ret = (m_PerformanceStats->getNbrTotalEpisodes() >= m_MinNbrEpisodes);
		ret &= (m_PerformanceStats->getMinNbrEpisodes() >= m_MinNbrEpisodesPerScenario);
		ret &= (m_PerformanceStats->getTotalSuccessPercentage() >= m_MinSuccessPercentage);
		ret &= (m_PerformanceStats->getMinSuccessPercentage() >= m_MinSuccessPercentagePerScenario);

		return ret;
	}
	else
	{
		return false;
	}
}
