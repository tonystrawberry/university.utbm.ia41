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


#ifndef AbstractVFunction_h
#define AbstractVFunction_h


#include "AL/Stimulus.h"


/*! \file AbstractVValueMemory.h
    \brief Abstract V-value Memory.
*/

/*! \def INTERFACE_ABSTRACT_VFUNCTION
    \brief Interface of a class inheriting from \a AbstractVFunction.
*/


//! Abstract V-Function.
/*!
    \author Fabrice LAURI
*/

class AbstractVFunction
{
    public:
        //! Get the name of the function.
        /*!
        */
        virtual const char* getName() const = 0;

        //! Reset the V-Function.
        /*!
        */
        virtual void reset() = 0;

        //! Get the value of the best response for a given stimulus.
        /*!
                \param x Stimulus.
        */
        virtual Ipseity::Scalar getValue( const SensorySignal & x ) = 0;

        //! Load the V-values.
        /*!
        */
        virtual bool load( const char* file_name ) = 0;

        //! Save the V-values.
        /*!
        */
        virtual bool save( const char* file_name ) = 0;
};


#define INTERFACE_ABSTRACT_VFUNCTION \
    const char* getName() const; \
    void reset(); \
    Ipseity::Scalar getValue( const SensorySignal & x ); \
    bool load( const char* ); \
    bool save( const char* ); \


#endif
