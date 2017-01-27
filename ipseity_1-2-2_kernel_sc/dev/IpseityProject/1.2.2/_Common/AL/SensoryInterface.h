/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2015, 2014, 2013 Ipseity Core Developers
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
 

#ifndef SensoryInterface_h
#define SensoryInterface_h


#include <vector>
#include "IpseityString.h"


class SensoryInterface
{
    public:
        //! Compare two sensory interfaces.
        /*!
         * \return Boolean: True if both are equal, False otherwise.
        */
        bool operator==( const SensoryInterface & si ) const;

        //! Compare two sensory interfaces.
        /*!
         * \return Boolean: True if both are different, False otherwise.
        */
        bool operator!=( const SensoryInterface & si ) const;

        //! Add a signal into the sensory interface.
        /*!
         * \param sensory_signal_name Name of the signal.
        */
        void add( const char* sensory_signal_name );

        //! Display the signals of the sensory interface.
        void display() const;

        //! Check the compatibility between two sensory interfaces.
        /*!
            \param sensory_interface Sensory interface.
            \return Boolean: True if compatibility is ok, False otherwise.
        */
        bool checkCompatibility( const SensoryInterface & sensory_interface ) const;


    public:
        std::vector<Ipseity::String> SensorySignals;
};


#endif
