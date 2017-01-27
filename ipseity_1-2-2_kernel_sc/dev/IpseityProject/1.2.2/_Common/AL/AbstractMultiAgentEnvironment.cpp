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


#include "AbstractMultiAgentEnvironment.h"

#include "AL/Agent.h"
#include "AL/XMLTags_Agents.h"

#include "MemDefns.h"
#include "IO/XMLScanner.h"
#include "IO/FileSearch.h"

#include <QDebug>


AbstractMultiAgentEnvironment::AbstractMultiAgentEnvironment() :
    AbstractBaseEnvironment(),
    m_AgentManager( NULL )
{
}


AbstractMultiAgentEnvironment::~AbstractMultiAgentEnvironment()
{
    clearAgents();
}


//*****************************************************************************


void
AbstractMultiAgentEnvironment::onInitialize()
{
}


//*****************************************************************************


bool
AbstractMultiAgentEnvironment::taxonExists( const char* taxon_name, uint32 & taxon_id ) const
{
    Taxon taxon;

    for( uint32 id = 0; id < getNbrTaxons(); ++id )
    {
        if (fetchTaxon( id, taxon ) && !strcmp( taxon.Name.c_str(), taxon_name ))
        {
            taxon_id = id;

            return true;
        }
    }

    return false;
}


//*****************************************************************************


bool
AbstractMultiAgentEnvironment::fetchCognitiveSystemTaxonId( const char* file_name, uint32 & taxon_id )
{
    if (file_name)
    {
        XMLScanner scanner_cs( file_name, "./" );

        if( !scanner_cs.isLoaded() )
        {
            return false;
        }

        XMLScanner::Token t;
        Ipseity::String taxon_name;

        while( scanner_cs.nextToken( &t ) )
        {
            switch( t.kind )
            {
                case XMLScanner::Token::Tag:
                    t.queryTagId( g_Tags );
                    break;

                case XMLScanner::Token::Attribute:
                    switch( t.tagId )
                    {
                        case TAG_COGNITIVE_SYSTEM:
                            if( t.keyIs( "Taxon" ) )
                            {
                                taxon_name = t.stringValue();
                            }
                            break;
                    }
                    break;
            }
        }

        return taxonExists( taxon_name.c_str(), taxon_id );
    }

    return false;
}


//*****************************************************************************
// Agent management
//*****************************************************************************


void
AbstractMultiAgentEnvironment::setAgentManager( AbstractAgentManager* am_pointer )
{
    m_AgentManager = am_pointer;
}


uint32
AbstractMultiAgentEnvironment::getNbrAgents() const
{
    return m_Agents.size();
}


AbstractAgent*
AbstractMultiAgentEnvironment::getAgent( uint32 agent_id )
{
    if (agent_id < getNbrAgents())
    {
        return m_Agents[agent_id];
    }
    else
    {
        qCritical() << "Return null agent!";
        return NULL;
    }
}


bool
AbstractMultiAgentEnvironment::addAgent( uint32 agent_id, uint32 taxon_id )
{
    if (agent_id >= getNbrAgents())
    {
        m_Agents.resize( agent_id+1, NULL );

//        cout << "allocate " << agent_id+1 << " agents" << endl;
    }
    else
    {
        SafeDelete( m_Agents[agent_id] );
//        cout << "delete agent: " << agent_id << endl;
    }


    m_Agents[agent_id] = new Agent( taxon_id );

    if (m_AgentIdPerTaxon.find( taxon_id ) == m_AgentIdPerTaxon.end())
    {
        m_AgentIdPerTaxon[taxon_id] = 1;
    }
    else
    {
        ++m_AgentIdPerTaxon[taxon_id];
    }

    Taxon taxon;
    Ipseity::String name;

    if (fetchTaxon( taxon_id, taxon ))
    {
        name = taxon.Name;
    }
    else
    {
        name = "Global";
    }
    name += "-";
    name += m_AgentIdPerTaxon[taxon_id];

    m_Agents[agent_id]->setName( name.c_str() );

    if (m_SystemSchedulingModule)
    {
        m_SystemSchedulingModule->init( getNbrAgents() );
    }

    if (m_AgentManager)
    {
        m_AgentManager->onAgentAdded( agent_id, taxon_id );
    }

    return true;
}


bool
AbstractMultiAgentEnvironment::addAgent( uint32 taxon_id )
{
    return addAgent( m_Agents.size(), taxon_id );
}


bool
AbstractMultiAgentEnvironment::removeAgent( uint32 agent_id )
{
    SafeDelete( m_Agents[agent_id] );
    m_Agents[agent_id] = NULL;

    if (m_AgentManager)
    {
        m_AgentManager->onAgentRemoved( agent_id );
    }

    return true;
}


void
AbstractMultiAgentEnvironment::clearAgents()
{
    std::vector<AbstractAgent*>::iterator itr;

    for( itr = m_Agents.begin(); itr != m_Agents.end(); ++itr )
    {
        SafeDelete( (*itr) );
    }
    m_Agents.clear();
    m_AgentIdPerTaxon.clear();

    if (m_SystemSchedulingModule)
    {
        m_SystemSchedulingModule->init( getNbrAgents() );
    }
}


//*****************************************************************************


bool
AbstractMultiAgentEnvironment::isScenarioSucceededByAgent( uint32 agent_id )
{
    return (m_Agents[agent_id] && m_Agents[agent_id]->isGoalReached());
}


//*****************************************************************************


bool
AbstractMultiAgentEnvironment::isCurrentAgentConscious()
{
    if (m_AgentId < getNbrAgents())
    {
        return (m_Agents[m_AgentId] && !isIdle( m_AgentId ));
    }
    else
    {
        return false;
    }
}
