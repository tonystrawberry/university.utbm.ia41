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


#ifndef AbstractQFunction_h
#define AbstractQFunction_h


#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "IO/AbstractPlugin.h"


/*! \file AbstractQFunction.h
    \brief Abstract Q-Function.
*/

/*! \def INTERFACE_ABSTRACT_QFUNCTION
    \brief Interface of a class inheriting from \a AbstractQFunction.
*/


//! Abstract Q-Function.
/*!
    The aim of a learning agent is to build a policy, that is a function mapping states to actions.
    A way to implicitly build this policy is to store for each stimulus-response pair \a (x,u) a Q-value Q(x,u)
    which represents the expected cumulative rewards received in stimulus \a x when carrying out response \a u
    and to take for a stimulus \a x the response \a u* that maximises Q(x,u') over all the possible responses \a u' in \a x.
    A Q-Function class provides methods to access and updates these Q-values.
    \author Fabrice LAURI
*/


class AbstractQFunction : public AbstractPlugin
{
    public:
        //! Init the memory.
        /*!
        */
        virtual void init( SensorySignalProperties*, ResponseSignalProperties* ) = 0;

        //! Get the sensory properties.
        /*!
        */
        virtual SensorySignalProperties* getSensorySignalProperties() const = 0;

        //! Get the response properties.
        /*!
        */
        virtual ResponseSignalProperties* getResponseSignalProperties() const = 0;

        //! Can the Q-value memory be reset.
        /*!
        */
        virtual bool canReset() const = 0;

        //! Reset the Q-value memory.
        /*!
        */
        virtual void reset() = 0;

        //! Get the number of cells.
        /*!
        */
        virtual uint32 getNbrCells() const = 0;

        //! Get the value of a state-action pair.
        /*!
            \param x Stimulus.
            \param u Response.
        */
        virtual Ipseity::Scalar getValue( const SensorySignal & x, const Response & u ) = 0;

        //! Update the Q-values of every visited stimulus-response pair using a temporal-difference estimate. Elibility traces are also taken into account while updating.
        /*!
            \param td Estimate of the temporal-difference.
        */
        virtual void update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate ) = 0;

        //! Clear the eligibility traces.
        /*!
        */
        virtual void clearEligibilityTraces() = 0;

        //! Decay the eligibility traces by a given factor.
        /*!
            \param factor Factor of decay.
        */
        virtual void decayEligibilityTraces( Ipseity::Scalar factor ) = 0;

        //! Replace the eligibility traces of a stimulus-response pair.
        /*!
        */
        virtual void replaceEligibilityTraces( const SensorySignal & x, const Response & u ) = 0;

        //! Accumulate the eligibility traces of a stimulus-response pair.
        /*!
        */
        virtual void accumulateEligibilityTraces( const SensorySignal & x, const Response & u ) = 0;

        //! Load the Q-values.
        /*!
        */
        virtual bool load( const char* file_name ) = 0;

        //! Save the Q-values.
        /*!
        */
        virtual bool save( const char* file_name ) = 0;
};


#define INTERFACE_ABSTRACT_QFUNCTION \
    void init( SensorySignalProperties*, ResponseSignalProperties* ); \
    SensorySignalProperties* getSensorySignalProperties() const; \
    ResponseSignalProperties* getResponseSignalProperties() const; \
    bool canReset() const; \
    void reset(); \
    uint32 getNbrCells() const; \
    Ipseity::Scalar getValue( const SensorySignal & x, const Response & u ); \
    void update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate ); \
    void clearEligibilityTraces(); \
    void decayEligibilityTraces( Ipseity::Scalar ); \
    void replaceEligibilityTraces( const SensorySignal & x, const Response & u ); \
    void accumulateEligibilityTraces( const SensorySignal & x, const Response & u ); \
    bool load( const char* ); \
    bool save( const char* ); \


#endif
