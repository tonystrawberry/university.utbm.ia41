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


#ifndef AbstractMultiAgentEnvironment_h
#define AbstractMultiAgentEnvironment_h


#include "AL/AbstractBaseEnvironment.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "AL/AbstractAgent.h"
#include "AL/PerformanceStats.h"

#include "IpseityString.h"

#include <vector>
#include <map>


//! Abstract multi-agent environment
/*!
    \brief Manage the agents.
    \author Fabrice LAURI
*/

class AbstractMultiAgentEnvironment : public AbstractBaseEnvironment
{
    public: // Dll interface
        AbstractMultiAgentEnvironment();
        ~AbstractMultiAgentEnvironment();

        // Cognitive systems
        bool fetchCognitiveSystemTaxonId( const char* file_name, uint32 & taxon_id );

        // Agents
        void setAgentManager( AbstractAgentManager* am_pointer );
        uint32 getNbrAgents() const;
        AbstractAgent* getAgent( uint32 agent_id );
        bool addAgent( uint32 agent_id, uint32 taxon_id );
        bool addAgent( uint32 taxon_id );
        bool removeAgent( uint32 agent_id );
        void clearAgents();

        bool isScenarioSucceededByAgent( uint32 agent_id );


    protected: // virtual functions
        //! Add an agent influence to the environment.
        /*!
            \param agent_id Agent Identifier.
            \param influence Response representing the influence of the agent on the environment.
        */
        virtual void addInfluence( uint32 agent_id, const Response & influence ) = 0;

    protected:
        void onInitialize();

        bool isCurrentAgentConscious();


    protected:
        void prepareAgents();

        bool taxonExists( const char* taxon_name, uint32 & taxon_id ) const;

        void onUpdateScenarii();


    protected:
        // Agents
        AbstractAgentManager* m_AgentManager;
        std::vector<AbstractAgent*> m_Agents;
        std::map<uint32,uint32> m_AgentIdPerTaxon;
};


#define INTERFACE_ABSTRACT_MULTI_AGENT_ENVIRONMENT \
    const QIcon & getIcon() const; \
    bool sendCommand( const char* command ); \
    bool pressKey( QKeyEvent* ev ); \
    uint32 getNbrEpisodes() const; \
    bool requireScenarioFile() const; \
    uint32 getNbrTaxons() const; \
    bool fetchTaxon( uint32 taxon_id, Taxon & taxon ) const; \
    uint32 getNbrSensoryInterfaces() const; \
    bool fetchSensoryInterface( uint32 sensory_interface_id, SensoryInterface & sensory_interface ) const; \
    bool checkSensoryInterfaceCompatibility( uint32 sensory_interface_id, const SensoryInterface & sensory_interface ) const; \
    bool isIdle( uint32 agent_id ) const; \
    const Stimulus & getCurrentStimulus( uint32 agent_id ); \
    const PerformanceMeasure & getAgentPerformance( uint32 agent_id ); \
    void initialize( const char* base_path, const char* environment_instance_name, const char* config_filename ); \
    void initRenderer(); \
    void quitRenderer(); \
    uint32 getNbrMainRenderingWindows() const; \
    uint32 getNbrScenarioRenderingWindows() const; \
    const char* getWindowName( uint32 window_id ); \
    void resizeWindow( uint32 window_id, int32, int32 ); \
    void renderWindow( uint32 window_id, QGLWidget* ); \


#endif
