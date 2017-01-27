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


#ifndef AbstractBehaviorModule_h
#define AbstractBehaviorModule_h


#include "AL/IpseityTypes.h"
#include "AL/AbstractResponseModule.h"
#include "AL/AbstractPolicy.h"
#include "AL/AbstractQFunction.h"
#include "IO/AbstractPlugin.h"


//! \file AbstractRLBehaviorModule.h
/*!
*/

/*! \def INTERFACE_ABSTRACT_RL_BEHAVIOR_MODULE
    \brief Interface (list of member functions) of a Reinforcement Learning Behavior Module.
*/


//! Abstract RL Behavior Module.
/*!
    A RL Behavior Module implements the methods used by an agent to select a response
    when a given stimulus is perceived.
    Epsilon-Greedy, Soft-Max are some examples of RL Behavior Modules.
    \author Fabrice LAURI
*/

class AbstractRLBehaviorModule : public AbstractPlugin
{
    public:
        //! Init the Behavior Module.
        /*!
            \param data_channel_id Data channel.
            \param policy Policy.
        */
		virtual void init( uint32 data_channel_id, AbstractPolicy* policy, AbstractQFunction* q_function )	= 0;

        //! Load the parameters of the Behavior Module.
        /*!
            \param file_name Name of the parameter file.
            \param base_path Search the file from this base path.
        */
        virtual void loadParameters( const char* file_name, const char* base_path ) = 0;

        //! Select the action that will be carried out by an agent for a given stimulus.
        /*!
            \param s Stimulus.
        */
        virtual const Response & selectResponse( const Stimulus & x ) = 0;

        //! Indicate whether a previous call to the member function \a selectResponse chose the best action or not.
        /*!
            \return
                - true if the best action was selected by a previous call to \a selectResponse. The best action corresponds to the action that maximizes the return for a given stimulus, according to the Q-value memory.
                - false otherwise.
            \see selectResponse
        */
        virtual bool isBestResponseSelected() const = 0;
};


#define INTERFACE_ABSTRACT_RL_BEHAVIOR_MODULE \
	void init( uint32 data_channel_id, AbstractPolicy* policy, AbstractQFunction* q_function ); \
    void loadParameters( const char*, const char* ); \
    const Response & selectResponse( const Stimulus & ); \
    bool isBestResponseSelected() const;


#endif
