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


#ifndef AbstractResponseModule_h
#define AbstractResponseModule_h


#include "AL/IpseityTypes.h"
#include "AL/Response.h"


//! \file AbstractResponseModule.h
/*!
*/


//! Abstract Response Module.
/*!
    A Response Module gives access to the possible responses available for a given sensory signal.
	\author Fabrice LAURI
*/


class AbstractResponseModule
{
    public:
		//! Set the properties (space) of the response (component names, ranges and intervals).
		/*!
			\param rp Vector of properties.
		*/
        virtual void setResponseSignalProperties( ResponseSignalProperties* rp ) = 0;

        //! Select the first response an agent can carry out when perceiving a given sensory signal.
        /*!
			\param x Stimulus.
        */
        virtual const Response & selectFirstResponse( const SensorySignal & x ) = 0;

        //! Select the next action an agent can do for a given sensory signal.
        /*!
            \param s Sensory signal.
            \param a Next action.
            \return
                - true if the agent can do another action.
                - false if the action \a a is the last possible action.
        */
        virtual bool selectNextResponse( const SensorySignal & s, Response & a )	= 0;

        //! Select one random action an agent can do in a given state.
        /*!
            \param s Sensory signal.
        */
        virtual const Response & selectRandomResponse( const SensorySignal & s ) = 0;
};


#define INTERFACE_ABSTRACT_RESPONSE_MODULE \
    void setResponseSignalProperties( ResponseSignalProperties* rp ); \
    const Response &  selectFirstResponse( const SensorySignal & s ); \
    bool selectNextResponse( const SensorySignal & s, Response & a ); \
    const Response &  selectRandomResponse( const SensorySignal & s ); \


#endif
