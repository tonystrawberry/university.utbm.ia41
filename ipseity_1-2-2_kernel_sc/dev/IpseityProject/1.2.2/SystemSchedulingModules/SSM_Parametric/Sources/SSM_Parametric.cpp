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


#include "SSM_Parametric.h"


SSM_Parametric::SSM_Parametric() :
    m_NbrAgents( 0 ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );

    m_Layout = new QGridLayout();
    m_Layout->setAlignment( Qt::AlignLeft );

    m_ComboboxEnvironmentClass = new ParameterComboBox( "SystemSchedulingClass", "System Scheduling:" );
    m_ComboboxEnvironmentClass->addItem( "Static" );
    m_ComboboxEnvironmentClass->addItem( "Implicitly Stigmergic" );
    m_ComboboxEnvironmentClass->addItem( "Dynamic" );
    m_ComboboxEnvironmentClass->addItem( "Chaotic" );

    m_ButtonGroupEnvironmentUpdateDelay = new QGroupBox( "Environment Update Delay (in milliseconds)" );
    m_ButtonGroupEnvironmentUpdateDelay->setFixedHeight( 100 );
    m_EnvironmentUpdateDelay = new Delay( m_ButtonGroupEnvironmentUpdateDelay );
    m_EnvironmentUpdateDelayTrigger = new DelayTrigger( m_EnvironmentUpdateDelay );

    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE] = new QGroupBox( "Agent Perception Delay (in milliseconds)" );
    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE]->setFixedHeight( 100 );
    m_AgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE] = new Delay( m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE] );

    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_ACT] = new QGroupBox( "Agent Action Delay (in milliseconds)" );
    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_ACT]->setFixedHeight( 100 );
    m_AgentDelay[IPSEITY_ENUM_AGENT_ACT] = new Delay( m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_ACT] );

    m_Layout->addWidget( m_ComboboxEnvironmentClass, 0,0 );
    m_Layout->addWidget( m_ButtonGroupEnvironmentUpdateDelay, 1,0 );
    m_Layout->addWidget( m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE], 2,0 );
    m_Layout->addWidget( m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_ACT], 2,1 );
    connect( m_ComboboxEnvironmentClass, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectType(int)) );
    onSelectType( 0 );
}


SSM_Parametric::~SSM_Parametric()
{
    if (m_Layout)
    {
        clearParameters();

        delete m_EnvironmentUpdateDelay;
        delete m_ButtonGroupEnvironmentUpdateDelay;

        delete m_Layout;
    }
}


const char*
SSM_Parametric::getBaseName() const
{
    return "SSM_Parametric";
}


const char*
SSM_Parametric::getDescription() const
{
    return "Parametric System Scheduling.\nSimulates the agents' interactions.";
}


const char*
SSM_Parametric::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
SSM_Parametric::defineParameters()
{
}


bool
SSM_Parametric::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}


bool
SSM_Parametric::update( uint32 current_tick, float64 current_time, float64 elapsed_time )
{
    switch( m_ComboboxEnvironmentClass->currentIndex() )
    {
        case SS_CHAOTIC:
            return m_EnvironmentUpdateDelayTrigger->timeout( elapsed_time );

        default:
            return true;
    }
}


void
SSM_Parametric::init( uint32 nbr_agents )
{
    clear();

    Delay* delay;

    for( uint32 interaction_signal = 0; interaction_signal < IPSEITY_ENUM_AGENT_NBR_SIGNALS; ++interaction_signal )
    {
        m_AgentInteractionSet[interaction_signal].resize( nbr_agents );
        delay = m_AgentDelay[interaction_signal];

        for( uint32 agent_id = 0; agent_id < nbr_agents; ++agent_id )
        {
            m_AgentInteractionSet[interaction_signal][agent_id] = new DelayTrigger( delay );
        }
    }

    m_NbrAgents = nbr_agents;
//    m_Label->setText( QString( "# agents: %1" ).arg( m_NbrAgents ) );
}


bool
SSM_Parametric::start()
{
    return true;
}


void
SSM_Parametric::end()
{
}


const AgentSet &
SSM_Parametric::getAgentSet( uint32 interaction_signal, uint32 current_tick, float64 current_time, float64 elapsed_time )
{
    m_AgentSet.clear();

    for( uint32 agent_id = 0; agent_id < m_NbrAgents; ++agent_id )
    {
        if (agentInteracts( interaction_signal, 0, agent_id, current_tick, elapsed_time ))
        {
            m_AgentSet.push_back( agent_id );
        }
    }

    return m_AgentSet;
}


//*****************************************************************************


void
SSM_Parametric::clear()
{
    for( uint32 interaction_signal = 0; interaction_signal < IPSEITY_ENUM_AGENT_NBR_SIGNALS; ++interaction_signal )
    {
        for( uint32 agent_id = 0; agent_id < m_AgentInteractionSet[interaction_signal].size(); ++agent_id )
        {
            delete m_AgentInteractionSet[interaction_signal][agent_id];
        }

        m_AgentInteractionSet[interaction_signal].clear();
    }
}


bool
SSM_Parametric::agentInteracts( uint32 interaction_signal, uint32 taxon_id, uint32 agent_id, uint32 current_tick, float64 elapsed_time )
{
    switch( m_ComboboxEnvironmentClass->currentIndex() )
    {
        case SS_STATIC:
            return true;

        case SS_STIGMERGIC:
            return (agent_id == (current_tick % m_NbrAgents));

        case SS_DYNAMIC:
        case SS_CHAOTIC:
            return m_AgentInteractionSet[interaction_signal][agent_id]->timeout( elapsed_time );
    }
}


//***************************************************************************************


void
SSM_Parametric::onSelectType( int index )
{
    m_ButtonGroupEnvironmentUpdateDelay->setEnabled( (index == 3) );

    bool enable_gb = (index >= SS_DYNAMIC);

    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_PERCEIVE]->setEnabled( enable_gb );
    m_ButtonGroupAgentDelay[IPSEITY_ENUM_AGENT_ACT]->setEnabled( enable_gb );
}
