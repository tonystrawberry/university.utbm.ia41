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


#ifndef MultiAgentEnvironment_h
#define MultiAgentEnvironment_h


#include "AL/IpseityTypes.h"
#include "AL/AbstractMultiAgentEnvironment.h"
#include "AL/AbstractMultiAgentEnvironmentInstance.h"
#include "IO/PluginWrapper.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterSpinBox.h"

#include <QtGui>


class MultiAgentEnvironment : public AbstractMultiAgentEnvironment
{
    public: // Dll interface
        MultiAgentEnvironment();
        ~MultiAgentEnvironment();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_MULTI_AGENT_ENVIRONMENT


    protected:
        void integrateInfluences( float64 elapsed_milliseconds );
        void addInfluence( uint32 agent_id, const Response & influence );
        void onEndEpisode();
        void load();

        const char* fetchScenarioName( uint32 scenario_id );
        const char* fetchScenarioDescription( uint32 scenario_id );
        uint64 fetchScenarioUid( uint32 scenario_id );

		void loadParameters();
		void saveParameters();


    protected:
        Ipseity::String m_EnvironmentInstanceName;
		PluginWrapper<AbstractMultiAgentEnvironmentInstance> m_PcMAE;
        AbstractMultiAgentEnvironmentInstance* m_MultiAgentEnvironment;

		// Statistics
		FILE* m_StatFile;
		FILE* m_EpisodeStatFile;

		uint32 m_NbrTotalEpisodes;
		QTime m_Time;

        Stimulus m_Stimulus;

		// GUI
        QGridLayout* m_Layout;

        QGroupBox* m_GroupBoxGeneralParameters;
        QGridLayout* m_LayoutStatistics;
        ParameterCheckBox* m_CheckboxSaveStats;
        ParameterCheckBox* m_CheckboxSaveEpisodeStats;
        ParameterCheckBox* m_CheckboxMaxIterations;
        ParameterSpinBox* m_SpinboxMaxIterations;

        QGroupBox* m_GroupBoxSpecificParameters;
        QGroupBox* m_GroupBoxDisturbanceParameters;

        PerformanceMeasure m_VoidPerformance;
};


#endif
