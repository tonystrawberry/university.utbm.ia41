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


#ifndef AbstractCognitiveSystem_h
#define AbstractCognitiveSystem_h


#include "AL/AbstractEnvironment.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "AL/Taxon.h"
#include "IO/AbstractPlugin.h"
#include "IO/InputManager.h"

#include <ostream>
#include <vector>


typedef struct
{
    bool Shared;
    uint32 Id;
} CognitiveSystemType;


//! Abstract CognitiveSystem.
/*!
    \brief Abstraction of a cognitive system.
    \author Fabrice LAURI
*/

class AbstractCognitiveSystem : public AbstractPlugin
{
    public:
        //! Initialize the environment of the cognitive system.
        virtual void setEnvironment( AbstractEnvironment* environment ) = 0;

        //! Initialize the cognitive system.
        /*!
            \param base_path Path of the cognitive system description file.
            \param file_name Description file of the cognitive system.
            \param taxon Associated taxon.
        */
        virtual bool init( const char* base_path, const char* file_name, const Taxon & taxon ) = 0;

        //! Set the mode of the cognitive system.
        /*!
         *  \param learning_mode If true, the cognitive system is learning, otherwise it is in the evaluation mode.
        */
        virtual void setMode( bool learning_mode ) = 0;

        //! Can the cognitive system be reset.
        /*!
         *  \return True if reset is permitted, false otherwise.
        */
        virtual bool canReset() const = 0;

        //! Reset the cognitive system, that is erase the memory of the cognitive system.
        /*!
        */
        virtual void reset() = 0;

        //! Get the number of agents associated with the cognitive system.
        /*!
            \return Number of agents.
        */
        virtual uint32 getNbrAgents() const = 0;

        //! Inform the cognitive system a new simulation has started.
        /*!
        */
        virtual void startNewSimulation() = 0;

        //! Indicate the end of a simulation.
        /*!
        */
        virtual void endSimulation() = 0;

        //! Inform the cognitive system a new episode has started.
        /*!
         *  \param scenario_uid Unique id of the scenario for which a new episode has just started.
        */
        virtual void startNewEpisode( uint32 nbr_total_episodes, uint64 scenario_uid ) = 0;

        //! Indicate the end of the episode.
        /*!
            \param True if terminated with success, false otherwise.
        */
        virtual void endEpisode( bool is_success ) = 0;

        //! Associate an agent with the cognitive system.
        /*!
            \param agent_id Id of the agent.
            \param agent_name Agent name.
        */
        virtual void linkAgent( uint32 agent_id, const char* agent_name ) = 0;

        //! Perceive a stimulus composed of sensory data.
        /*!
         *  \param agent_id Id of the agent perceiving the stimulus.
         *  \param x Current stimulus.
        */
        virtual void perceive( uint32 agent_id, const Stimulus & x ) = 0;

        //! Select the response to carry out at the current time.
        /*!
            \param agent_id Id of the agent.
            \return Chosen response.
        */
        virtual const Response & selectResponse( uint32 agent_id, bool last_decision ) = 0;

        //! A key has been pressed.
        /*!
            \param key_code Code of the pressed key.
        */
        virtual void pressKey( int32 key_code ) = 0;

        //! A key has been released.
        /*!
            \param key_code Code of the released key.
        */
        virtual void releaseKey( int32 key_code ) = 0;
};


#define INTERFACE_ABSTRACT_COGNITIVE_SYSTEM \
    void setEnvironment( AbstractEnvironment* environment ); \
    bool init( const char* base_path, const char* name, const Taxon & taxon ); \
    void setMode( bool learning_mode ); \
    bool canReset() const; \
    void reset(); \
    uint32 getNbrAgents() const; \
    void linkAgent( uint32 agent_id, const char* agent_name ); \
    void startNewSimulation(); \
    void endSimulation(); \
    void startNewEpisode( uint32 nbr_total_episodes, uint64 scenario_uid ); \
    void endEpisode( bool is_success ); \
    void perceive( uint32 agent_id, const Stimulus & ); \
    const Response & selectResponse( uint32 agent_id, bool last_decision ); \
    void pressKey( int32 key_code ); \
    void releaseKey( int32 key_code ); \


#endif
