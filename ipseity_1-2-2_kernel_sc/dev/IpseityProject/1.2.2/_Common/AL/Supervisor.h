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


#ifndef Supervisor_h
#define Supervisor_h


#include "AL/AbstractSupervisor.h"

#include "PerformanceStats.h"
#include "Scenarii.h"


class Supervisor : public AbstractSupervisor
{
	public:
		Supervisor();

		void setScenarii( Scenarii* scenarii );
		void setPerformanceStats( PerformanceStats* ps );

		bool DllCallConv isEnabled() const;
		void DllCallConv setEnabled( bool );

		bool DllCallConv isReady() const;
		void DllCallConv setReady( bool );

		uint32 DllCallConv getMinNbrEpisodes() const;
		void DllCallConv setMinNbrEpisodes( uint32 nbr );
		uint32 DllCallConv getMinNbrEpisodesPerScenario() const;
		void DllCallConv setMinNbrEpisodesPerScenario( uint32 nbr );
		float64 DllCallConv getMinSuccessPercentage() const;
		void DllCallConv setMinSuccessPercentage( float64 perc );
		float64 DllCallConv getMinSuccessPercentagePerScenario() const;
		void DllCallConv setMinSuccessPercentagePerScenario( float64 perc );

		uint32 selectScenario() const;

		bool isPerformanceReached() const;


	protected:
		Scenarii* m_Scenarii;
		PerformanceStats* m_PerformanceStats;

		bool m_Enabled;

		bool m_Ready;
		uint32 m_MinNbrEpisodes;
		uint32 m_MinNbrEpisodesPerScenario;
		float64 m_MinSuccessPercentage;
		float64 m_MinSuccessPercentagePerScenario;
};


#endif
