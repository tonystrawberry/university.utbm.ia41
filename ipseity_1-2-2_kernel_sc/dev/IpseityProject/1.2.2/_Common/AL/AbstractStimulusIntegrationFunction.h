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


#ifndef AbstractStimulusIntegrationFunction_h
#define AbstractStimulusIntegrationFunction_h


#include "IO/AbstractPlugin.h"
#include "AL/Sensation.h"
#include "AL/Stimulus.h"


//! Abstraction of a function transforming stimuli into sensations.
/*!
 *  \author Fabrice LAURI
*/


class AbstractStimulusIntegrationFunction : public AbstractPlugin
{
	public:
        //!  Build a sensation from a stimulus.
        /*!
         * \param stimulus Stimulus to integrate.
         * \return The current sensation.
         */
        virtual const Sensation & integrate( const Stimulus & stimulus ) = 0;
};


#define ABSTRACT_STIMULUS_INTEGRATION_FUNCTION \
    const Sensation & integrate( const Stimulus & stimulus ); \


#endif
