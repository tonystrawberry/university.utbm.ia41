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


#ifndef AbstractPolicy_h
#define AbstractPolicy_h


#include "AL/IpseityTypes.h"
#include "AL/Response.h"


//! Abstract Policy.
/*!
    The aim of a learning agent is to build a policy, that is a function mapping states to actions.
    A policy must inherits from this class.
    \author Fabrice LAURI
*/


class AbstractPolicy
{
    public:
		//! Select the best response for a given stimulus.
        /*!
            \param x Stimulus.
        */
        virtual const Response & selectBestResponse( const SensorySignal & x ) = 0;

		//! Select a random response for a given stimulus.
		/*!
			\param x Stimulus.
		*/
        virtual const Response & selectRandomResponse( const SensorySignal & x ) = 0;
};


#define INTERFACE_ABSTRACT_POLICY \
    const Response & selectBestResponse( const SensorySignal & x ); \
    const Response & selectRandomResponse( const SensorySignal & x ); \


#endif
