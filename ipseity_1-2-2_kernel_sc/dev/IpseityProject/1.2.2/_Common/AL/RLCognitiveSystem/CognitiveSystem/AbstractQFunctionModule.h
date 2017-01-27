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


#ifndef AbstractQFunctionModule_h
#define AbstractQFunctionModule_h


#include "AL/IpseityTypes.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "Math/Vector.h"
#include "IO/AbstractPlugin.h"

#include <stdio.h>


//! Abstract Q-Function Module.
/*!
    \author Fabrice LAURI
*/

class AbstractQFunctionModule : public AbstractPlugin
{
    public:
        //! Is the number of cells variable?
        /*!
        */
        virtual bool isDynamic() const = 0;

        //! Load the parameters of the Q-value memory module.
        /*!
                \param base_path Relative path.
                \param name File name.
        */
        virtual bool loadParameters( const char* name, const char* base_path ) = 0;

        virtual void setDataDescription( SensorySignalProperties*, ResponseSignalProperties* ) = 0;

        //! Reset the Q-value Memory.
        /*!
        */
        virtual bool canReset() const = 0;

        //! Reset the Q-value Memory.
        /*!
        */
        virtual void reset() = 0;

        //! Get the number of cells.
        /*!
        */
        virtual uint32 getNbrCells() const = 0;

        //! Get the value of a state-action pair.
        /*!
                \param s Stimulus.
                \param a Response.
        */
        virtual Ipseity::Scalar getValue( const SensorySignal & x, const Response & u ) = 0;

        virtual uint32 getNbrVisits( const SensorySignal & x, const Response & u ) = 0;

        //! Update the Q-values of every visited state-action pair using a temporal-difference estimate. Elibility traces are also taken into account while updating.
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

        //! Replace the eligibility traces of a state-action pair.
        /*!
        */
        virtual void replaceEligibilityTraces( const SensorySignal &, const Response & ) = 0;

        //! Accumulate the eligibility traces of a state-action pair.
        /*!
        */
        virtual void accumulateEligibilityTraces( const SensorySignal &, const Response & ) = 0;

        //! Load the Q-value memory.
        /*!
        */
        virtual bool load( const char* ) = 0;

        //! Save the Q-value memory.
        /*!
        */
        virtual bool save( const char* ) = 0;
};


#define INTERFACE_QVMM \
    bool isDynamic() const; \
    bool loadParameters( const char*, const char* ); \
    void setDataDescription( SensorySignalDescription*, ResponseSignalDescription* ); \
    bool canReset() const; \
    void reset(); \
    uint32 getNbrCells() const; \
    IpseityPrecisionType getValue( const SensorySignal &, const Response & ); \
    uint32 getNbrVisits( const SensorySignal & x, const Response & u ); \
    void update( IpseityPrecisionType ); \
    void clearEligibilityTraces(); \
    void decayEligibilityTraces( IpseityPrecisionType ); \
    void replaceEligibilityTraces( const SensorySignal &, const Response & ); \
    void accumulateEligibilityTraces( const SensorySignal &, const Response & ); \
    bool load( const char* ); \
    bool save( const char* ); \


#endif
