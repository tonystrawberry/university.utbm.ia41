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


#include "AbstractBaseEnvironment.h"

#include "IO/InputManager.h"
#include "IO/VFiles.h"
#include "IO/XMLScanner.h"
#include "AL/AbstractSystemScheduling.h"
#include "AL/XMLTags_Environments.h"

#include "StringFunctions.h"

#include <QDebug>


AbstractBaseEnvironment::AbstractBaseEnvironment() :
    m_CurrentScenarioLoaded( false ),
    m_MaxTicks( 0 ),
    m_MaxTime( 0.0 ),
    m_StepByStep( true ),
    m_SystemSchedulingModule( NULL )
{
}

AbstractBaseEnvironment::~AbstractBaseEnvironment()
{
}


const char*
AbstractBaseEnvironment::getBasePath() const
{
    return m_BasePath.c_str();
}


void
AbstractBaseEnvironment::setParameterFileName( const char* file_name )
{
    m_ParameterFileName = file_name;
}

const char*
AbstractBaseEnvironment::getParameterFileName() const
{
    return m_ParameterFileName.c_str();
}


uint32
AbstractBaseEnvironment::getMaxIterations() const
{
    return m_MaxTicks;
}

void
AbstractBaseEnvironment::setMaxIterations( uint32 max_ticks )
{
    m_MaxTicks = max_ticks;
}


float64
AbstractBaseEnvironment::getMaxSimulationTime() const
{
    return m_MaxTime;
}

void
AbstractBaseEnvironment::setMaxSimulationTime( float64 max_time )
{
    m_MaxTime = max_time;
}


uint32
AbstractBaseEnvironment::getNbrIterations() const
{
    return m_NbrTicks;
}


//*****************************************************************************


uint32
AbstractBaseEnvironment::getSimulationStatus() const
{
    return m_SimulationStatus;
}


void
AbstractBaseEnvironment::faceCurrentScenario()
{
    m_NbrTicks = 0;
    m_CurrentTime = 0.0;
    m_CurrentScenarioLoaded = true;
    m_EpisodeEnded = false;
    m_SimulationHalted = false;

//    cout << "--- Clear agents" << endl;
    clearAgents();
//    cout << "--- Load environment" << endl;
    load();

//    cout << "Done!" << endl;
}


bool
AbstractBaseEnvironment::selectNewAgent()
{
    if (m_AgentItr == m_AgentSet[m_InteractionSignal].end())
    {
        ++m_InteractionSignal;

        if (m_InteractionSignal == IPSEITY_ENUM_AGENT_NBR_SIGNALS)
        {
            return false;
        }
        else
        {
            m_AgentItr = m_AgentSet[m_InteractionSignal].begin();
            m_AgentId = (*m_AgentItr);
            ++m_AgentItr;

            return true;
        }
    }
    else
    {
        m_AgentId = (*m_AgentItr);
        ++m_AgentItr;

        return true;
    }
}


//*****************************************************************************


void
AbstractBaseEnvironment::setSystemScheduling( AbstractSystemScheduling* system_scheduling )
{
    m_SystemSchedulingModule = system_scheduling;
}


//*****************************************************************************


void
AbstractBaseEnvironment::setExperiment( uint32 trial_id, uint32 episode_id, uint64 scenario_uid, const char* scenario_filename, bool learning_mode )
{
    m_TrialId = trial_id;
    m_EpisodeId = episode_id;
    m_ScenarioUid = scenario_uid;
    m_ScenarioFileName = scenario_filename;
    m_LearningMode = learning_mode;
}


void
AbstractBaseEnvironment::startSimulation()
{
    m_SimulationStatus = SS_START;
}


void
AbstractBaseEnvironment::stopSimulation( bool halted )
{
    if (halted)
    {
        m_SimulationStatus = SS_NEXT_EPISODE;
        m_SimulationHalted = true;
    }
    else
    {
        m_SimulationStatus = SS_END;

        if (m_SystemSchedulingModule)
        {
            m_SystemSchedulingModule->end();
        }

        onEndEpisode();
    }
}


bool
AbstractBaseEnvironment::continueSimulation()
{
    switch( m_SimulationStatus )
    {
        case SS_NEW_INTERACTION_STEP:
            return !m_StepByStep;

        case SS_END:
            return false;

        default:
            return true;
    }
}


uint32
AbstractBaseEnvironment::getSimulationAgentId() const
{
    return m_AgentId;
}


void
AbstractBaseEnvironment::enableStepByStep( bool flag )
{
    m_StepByStep = flag;
}


uint32
AbstractBaseEnvironment::simulate( float64 elapsed_milliseconds )
{
    switch( m_SimulationStatus )
    {
        case SS_START:
        case SS_NEW_SCENARIO:
            m_SimulationStatus = SS_FACE_CURRENT_SCENARIO;
            return m_SimulationStatus;

        case SS_FACE_CURRENT_SCENARIO:
            faceCurrentScenario();

            m_SimulationStatus = SS_PREPARE_NEW_EPISODE;
            return m_SimulationStatus;

        case SS_PREPARE_NEW_EPISODE:
            m_SimulationStatus = SS_NEW_INTERACTION_STEP;
            return m_SimulationStatus;

        case SS_NEW_INTERACTION_STEP:
            m_SimulationStatus = SS_ADD_INFLUENCES;
            return m_SimulationStatus;

        case SS_ADD_INFLUENCES:
        {
            if (m_SystemSchedulingModule)
            {
//                qDebug() << "--";
                if (m_SystemSchedulingModule->start())
                {
                }

                m_AgentSet[IPSEITY_ENUM_AGENT_PERCEIVE] = m_SystemSchedulingModule->getAgentSet( IPSEITY_ENUM_AGENT_PERCEIVE, m_NbrTicks, m_CurrentTime, elapsed_milliseconds );
                m_AgentSet[IPSEITY_ENUM_AGENT_ACT] = m_SystemSchedulingModule->getAgentSet( IPSEITY_ENUM_AGENT_ACT, m_NbrTicks, m_CurrentTime, elapsed_milliseconds );

                m_AgentItr = m_AgentSet[IPSEITY_ENUM_AGENT_PERCEIVE].begin();
                m_InteractionSignal = IPSEITY_ENUM_AGENT_PERCEIVE;

                m_SimulationStatus = SS_AGENTS_INTERACT;
            }
            else
            {
                m_SimulationStatus = SS_INTEGRATE_INFLUENCES;
            }
            return m_SimulationStatus;
        }

        case SS_AGENTS_INTERACT:
        {
            if (isEpisodeEnded())
            {
                m_SimulationStatus = SS_NEXT_EPISODE;
            }
            else
            {
                if (selectNewAgent())
                {
                    if (isCurrentAgentConscious())
                    {
                        switch (m_InteractionSignal)
                        {
                            case IPSEITY_ENUM_AGENT_PERCEIVE:
                                m_SimulationStatus = SS_AGENT_PERCEIVE;
                                break;

                            case IPSEITY_ENUM_AGENT_ACT:
                                m_SimulationStatus = SS_AGENT_ADD_INFLUENCE;
                                break;
                        }
                    }
                    else
                    {
                        m_SimulationStatus = SS_AGENT_REST;
                    }
                }
                else
                {
                    m_SimulationStatus = SS_INTEGRATE_INFLUENCES;
                }
            }

            return m_SimulationStatus;
        }

        case SS_AGENT_PERCEIVE:
        case SS_AGENT_ADD_INFLUENCE:
        case SS_AGENT_REST:
            m_SimulationStatus = SS_AGENTS_INTERACT;
            return m_SimulationStatus;

        case SS_INTEGRATE_INFLUENCES:
        {
            if (m_SystemSchedulingModule->update( m_NbrTicks, m_CurrentTime, elapsed_milliseconds ))
            {
                if (m_EpisodeInteractionsFile)
                {
                    std::list<uint32>::const_iterator itr;

                    if (m_AgentSet[IPSEITY_ENUM_AGENT_PERCEIVE].size())
                    {
                        fprintf( m_EpisodeInteractionsFile, "%10.4f", m_CurrentTime );
                        fprintf( m_EpisodeInteractionsFile, "%10d   ", m_NbrTicks );
                        for( itr = m_AgentSet[IPSEITY_ENUM_AGENT_PERCEIVE].begin(); itr != m_AgentSet[IPSEITY_ENUM_AGENT_PERCEIVE].end(); ++itr )
                        {
                            fprintf( m_EpisodeInteractionsFile, " %d", (*itr) );
                        }
                        fprintf( m_EpisodeInteractionsFile, " # Agents perceiving\n" );
                    }

                    if (m_AgentSet[IPSEITY_ENUM_AGENT_ACT].size())
                    {
                        fprintf( m_EpisodeInteractionsFile, "%10.4f", m_CurrentTime );
                        fprintf( m_EpisodeInteractionsFile, "%10d   ", m_NbrTicks );
                        for( itr = m_AgentSet[IPSEITY_ENUM_AGENT_ACT].begin(); itr != m_AgentSet[IPSEITY_ENUM_AGENT_ACT].end(); ++itr )
                        {
                            fprintf( m_EpisodeInteractionsFile, " %d", (*itr) );
                        }
                        fprintf( m_EpisodeInteractionsFile, " # Agents acting\n" );
                    }
                }

    //            qDebug() << "Integrate influences";

                ++m_NbrTicks;
                m_CurrentTime += elapsed_milliseconds;
                integrateInfluences( elapsed_milliseconds );
            }

            m_SimulationStatus = SS_NEW_INTERACTION_STEP;
            return m_SimulationStatus;
        }

        case SS_NEXT_EPISODE:
            onEndEpisode();
#ifdef TRACE
            cout << "Episode ended!" << endl;
#endif
//            ++m_NbrEpisodes;

            if (getNbrAgents() > 0)
            {
                m_AgentId = 0;
                m_SimulationStatus = SS_AGENT_END_EPISODE;
            }
            else
            {
                if (m_SimulationHalted)
                {
                    m_SimulationStatus = SS_SIMULATION_HALTED;
                }
                else
                {
                    m_SimulationStatus = SS_END_EPISODE;
                }
            }
            return m_SimulationStatus;

        case SS_AGENT_END_EPISODE:
            ++m_AgentId;
            if (m_AgentId >= getNbrAgents())
            {
                if (m_SimulationHalted)
                {
                    m_SimulationStatus = SS_SIMULATION_HALTED;
                }
                else
                {
                    m_SimulationStatus = SS_END_EPISODE;
                }
            }
            return m_SimulationStatus;

        case SS_END_EPISODE:
            m_SimulationStatus = SS_NEW_SCENARIO;
            return m_SimulationStatus;
    }

    m_SimulationStatus = SS_END;
    return m_SimulationStatus;
}


bool
AbstractBaseEnvironment::isSimulationTerminated() const
{
    return (m_SimulationStatus == SS_END);
}


bool
AbstractBaseEnvironment::isEpisodeEnded() const
{
    return m_EpisodeEnded;
}


//*****************************************************************************
