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


#ifndef AbstractAgent_h
#define AbstractAgent_h


#include "Types.h"


//! Abstract agent.
/*! \author Fabrice LAURI
*/

class AbstractAgent
{
    public:
        virtual ~AbstractAgent() {}

        //! Get the name of the agent.
        /*!
        */
        virtual const char* getName() const = 0;

        //! Set the agent's name.
        /*!
         *  \param name New name.
        */
        virtual void setName( const char* name ) = 0;

        //! Get the taxon id of the agent.
        /*!
        */
        virtual uint32 getTaxonId() const = 0;

        //! Does the agent encounter a terminal stimulus?
        /*!
        */
        virtual bool isTerminal() const = 0;

        //! Is the goal reached?
        /*!
        */
        virtual bool isGoalReached() const = 0;

        //! Set the interaction state of the agent (terminal, reached goal).
        /*!
         *  \param state Indicates if the agent is in a terminal state and if it has reached a goal.
        */
        virtual void setInteractionState( uint32 state ) = 0;
};


#define INTERFACE_ABSTRACT_AGENT \
    const char* getName() const; \
    void setName( const char* name ); \
    uint32 getTaxonId() const; \
    bool isTerminal() const; \
    bool isGoalReached() const; \
    void setInteractionState( uint32 state ); \


#endif
