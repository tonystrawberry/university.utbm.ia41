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


#ifndef AbstractSystemScheduling_h
#define AbstractSystemScheduling_h


#include "AL/IpseityTypes.h"
#include "IO/AbstractPlugin.h"

#include <list>


//! Enum for interaction signals.
/*!
 * \author Fabrice LAURI
*/

typedef enum {
    IPSEITY_ENUM_AGENT_PERCEIVE = 0,
    IPSEITY_ENUM_AGENT_ACT,

    IPSEITY_ENUM_AGENT_NBR_SIGNALS
} IpseityEnumAgentInteractionSignal;


//! Abstract System Scheduling.
/*!
 * The system scheduling controls the scheduling of perception and action of the agents, as well as when the environment must be updated.
 * \author Fabrice LAURI
*/

class AbstractSystemScheduling : public AbstractPlugin
{
    public:
        //! Must the environment be updated?
        /*!
         * \param current_tick Number of time ticks from the beginning of the episode.
         * \param current_time Elapsed time from the beginning of the episode.
         * \param elapsed_time Elapsed time since the last call to this function.
         * \return Boolean: True if the environment must be updated, False otherwise.
        */
        virtual bool update( uint32 current_tick, float64 current_time, float64 elapsed_time ) = 0;

        //! Initialize with the current number of agents.
        /*!
         * \param nbr_agents Number of agents.
        */
        virtual void init( uint32 nbr_agents ) = 0;

        //! Can the system scheduling be started?
        /*!
         * \return Boolean.
        */
		virtual bool start() = 0;

        //! End the system scheduling.
        /*!
        */
        virtual void end() = 0;

        //! Get the set of agents that must perceive or act.
        /*!
         * \param interaction_type Type of agents to get: agents that perceive or agents that act.
         * \param current_tick Number of time ticks from the beginning of the episode.
         * \param current_time Elapsed time from the beginning of the episode.
         * \param elapsed_time Elapsed time since the last call to this function.
         * \return Set of agents (array of integers).
        */
        virtual const AgentSet & getAgentSet( uint32 interaction_signal, uint32 current_tick, float64 current_time, float64 elapsed_time ) = 0;
};


#define INTERFACE_ABSTRACT_SYSTEM_SCHEDULING_MODULE \
    bool update( uint32 current_tick, float64 current_time, float64 elapsed_time ); \
    void init( uint32 nbr_agents ); \
	bool start(); \
    void end(); \
    const AgentSet & getAgentSet( uint32 interaction_signal, uint32 current_tick, float64 current_time, float64 elapsed_time ); \


#endif
