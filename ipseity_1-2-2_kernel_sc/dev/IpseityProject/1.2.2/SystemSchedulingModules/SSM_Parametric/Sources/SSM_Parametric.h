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


#ifndef SSM_Parametric_h
#define SSM_Parametric_h


#include "AL/IpseityTypes.h"
#include "AL/AbstractSystemScheduling.h"
#include "Math/Permutation.h"
#include "Delay.h"

#include <vector>
#include <QtGui>

#include "IO/ParameterComboBox.h"


enum
{
    SS_STATIC       = 0,
    SS_STIGMERGIC,
    SS_DYNAMIC,
    SS_CHAOTIC
};


class SSM_Parametric : public QObject, public AbstractSystemScheduling
{
	Q_OBJECT

    public: // Dll interface
        SSM_Parametric();
        ~SSM_Parametric();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_SYSTEM_SCHEDULING_MODULE

    protected:
        void clear();

        bool agentInteracts( uint32 interaction_signal, uint32 taxon_id, uint32 agent_id, uint32 current_tick, float64 elapsed_time );

    protected slots:
        void onSelectType(int);

    protected:
        uint32 m_NbrAgents;
        AgentSet m_AgentSet;
        DelayTrigger* m_EnvironmentUpdateDelayTrigger;
        std::vector<DelayTrigger*> m_AgentInteractionSet[IPSEITY_ENUM_AGENT_NBR_SIGNALS];

        // Gui
        QGridLayout* m_Layout;
        ParameterComboBox* m_ComboboxEnvironmentClass;
        QGroupBox* m_ButtonGroupEnvironmentUpdateDelay;
        Delay* m_EnvironmentUpdateDelay;
        QGroupBox* m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_NBR_SIGNALS];
        Delay* m_AgentDelay[IPSEITY_ENUM_AGENT_NBR_SIGNALS];
//        QLabel* m_Label;
};


#endif
