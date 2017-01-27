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


#ifndef AbstractLearningModule_h
#define AbstractLearningModule_h


#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "AL/AbstractAgent.h"
#include "AL/AbstractRLBehaviorModule.h"
#include "AL/AbstractVFunction.h"
#include "AL/AbstractQFunction.h"
#include "AL/AbstractPolicy.h"
#include "IO/AbstractPlugin.h"

#include <map>


//! Abstract Learning Module.
/*!
    A Learning Module implements the methods used by an agent to learn from interactions with its environment.
    An interaction is of the form \a <s,a,ns,r>, where \a s is the current state, \a a is the action carried out in state s,
    \a ns is the state reached after carrying out action \a a and \a r is the received reward from transition <s,a,ns>.
    \author Fabrice LAURI
*/

class AbstractLearningModule : public AbstractPlugin
{
    public:
        //! Init the Learning Module.
        /*!
            \param data_channel_id ChannelId in the stimulus of the perception data
            \param reward_channel_id ChannelId in the stimulus of the reward
            \param vvm V-value memory used to assign a value to the return of each stimulus.
            \param qvm Q-value memory used to assign a value to the return of each stimulus-response pair.
        */
        virtual void init( uint32 data_channel_id, uint32 reward_channel_id, Ipseity::Scalar gamma, AbstractVFunction* vvm, AbstractQFunction* qvm ) = 0;

        //! Load the parameters of the Learning Module.
        /*!
            \param name Name of the parameter file.
            \param base_path Search the file from this base path.
            \return true if the parameters have been loaded.
        */
        virtual bool loadParameters( const char* file_name, const char* base_path  ) = 0;

        //! Is the learning module an online policy method?
        /*!
        */
        virtual bool isOnlinePolicy() const = 0;

        //! Learn from a tuple <current_stimulus,current_response,reward,next_stimulus>.
        /*!
            \param current_stimulus Current stimulus.
            \param current_response Response carried out in stimulus current_stimulus.
            \param is_current_response_best Is the current response the best?
            \param next_stimulus_and_reward Stimulus reached after carrying out current_response in current_stimulus. Reward is inside next_stimulus_and_reward.
        */
        virtual void learn( const Stimulus & current_stimulus, const Response & current_response, bool is_current_response_best, const Stimulus & next_stimulus_and_reward )	= 0;

        //! Learn from a tuple <current_stimulus,current_response,reward,next_stimulus,next_response>.
        /*!
            \param current_stimulus Current stimulus.
            \param current_response Response carried out in stimulus current_stimulus.
            \param next_stimulus_and_reward Stimulus reached after carrying out current_response in current_stimulus. Reward is inside next_stimulus_and_reward.
            \param next_response Response carried out in stimulus next_stimulus.
        */
        virtual void learn( const Stimulus & current_stimulus, const Response & current_response, const Stimulus & next_stimulus_and_reward, const Response & next_response )	= 0;
};


#endif
