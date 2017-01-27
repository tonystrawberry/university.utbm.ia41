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


#ifndef AbstractSystemSchedulingInstance_h
#define AbstractSystemSchedulingInstance_h


//! Manage the insertion and deletion of agents.
/*!
    \author Fabrice LAURI
*/


class AbstractAgentManager
{
    public:
        //! Called when a new agent is added to the environment.
        /*!
         * \param agent_id Identifier of the agent.
         * \param taxon_id Taxon identifier of the agent.
         */
        virtual void onAgentAdded( uint32 agent_id, uint32 taxon_id );

        //! Called when an agent is removed from the environment.
        /*!
         * \param agent_id Identifier of the agent.
         */
        virtual void onAgentRemoved( uint32 agent_id );
};


#endif
