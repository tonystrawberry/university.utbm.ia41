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


#ifndef AbstractContinuousPolicy_h
#define AbstractContinuousPolicy_h


#include "AL/IpseityTypes.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "IO/AbstractPlugin.h"
#include "AL/AbstractPolicy.h"
#include "AL/AbstractLearningModule.h"
#include "AL/RLCognitiveSystem/Policy/BinaryDecisionSequence.h"


class AbstractContinuousPolicy : public AbstractPlugin, public AbstractPolicy
{
    public:
        //! Set the original Response Space description.
        /*!
            \param rd Response Description
        */
        virtual void setResponseDescription( const ResponseSignalProperties & rd ) = 0;

        //! Get the new response space
        /*!
        */
        virtual ResponseSignalProperties* getDecisionDescription() = 0;

        //! Initialize the response search module.
        /*!
            \param file_name Name of the description file.
            \param base_path Path of the description file.
        */
        virtual bool init( const char* file_name, const char* base_path ) = 0;

        //! Set the policy for selecting binary decisions.
        /*!
        */
        virtual void setPolicy( AbstractPolicy* ) = 0;

        //! Get the number of binary decisions
        /*!
        */
        virtual uint32 getNbrDecisions() const = 0;

        //! Set the number of binar decisions
        /*!
        */
        virtual void setNbrDecisions( uint32 nbr_decisions ) = 0;

        //! Get the Binary Decision Sequence generated when selecting a response
        /*!
        */
        virtual const BinaryDecisionSequence & getBinaryDecisionSequence() const = 0;
};


const uint32 CONTINUOUS_POLICY_INTERFACE_VERSION = 0x00010000;


#define INTERFACE_ABSTRACT_CONTINUOUS_POLICY \
    void setResponseDescription( const ResponseSignalProperties & rd ); \
    ResponseSignalProperties* getDecisionDescription(); \
    bool init(const char* file_name, const char* base_path); \
    void setPolicy( AbstractPolicy* ); \
    uint32 getNbrDecisions() const; \
    void setNbrDecisions( uint32 nbr_decisions ); \
    const BinaryDecisionSequence & getBinaryDecisionSequence() const; \


#endif
