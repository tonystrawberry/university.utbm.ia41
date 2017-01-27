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


#ifndef AbstractVersatileBehaviorModule_h
#define AbstractVersatileBehaviorModule_h


#include "AL/IpseityTypes.h"
#include "AL/Sensation.h"
#include "AL/AbstractResponseModule.h"
#include "IO/AbstractPlugin.h"

#include <vector>


//! \file AbstractVersatileBehaviorModule.h
/*!
*/

/*! \def INTERFACE_ABSTRACT_VERSATILE_BEHAVIOR_MODULE
    \brief Interface (list of member functions) of a Behavior Module.
*/


//! Abstract Versatile Behavior Module.
/*!
    A Behavior Module implements the methods used by an agent to select a response
    when a given stimulus is perceived.
    \author Fabrice LAURI
*/


class AbstractVersatileBehaviorModule : public AbstractPlugin
{
    public:
        //! Init the Behavior Module.
        /*!
            \param rm Response Module that handle the possible responses available for a stimulus.
        */
        virtual void init( const char* program_name, uint32 data_channel_id, AbstractResponseModule* rm ) = 0;

        //! Load the parameters of the Behavior Module.
        /*!
            \param file_name Name of the parameter file.
            \param base_path Search the file from this base path.
        */
        virtual void loadParameters( const char* file_name, const char* base_path ) = 0;

		//! Select the action that will be carried out by an agent when perceiving a given stimulus.
        /*!
			\param x Stimulus.
        */
        virtual const Response & selectResponse( const Sensation & x ) = 0;
};


#define INTERFACE_ABSTRACT_VERSATILE_BEHAVIOR_MODULE \
    void init( const char* program_name, uint32 data_channel_id, AbstractResponseModule* ); \
    void loadParameters( const char*, const char* ); \
    const Response & selectResponse( const Sensation & ); \


#endif
