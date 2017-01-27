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


#ifndef Scenarii_h
#define Scenarii_h


#include "Types.h"
#include "IpseityString.h"

#include <vector>


//! Set of scenarii that can be executed within an environment.
/*!
    A scenario refers to a set of initial states of an environment.
    \author Fabrice LAURI
*/


class Scenarii
{
    public:
        //! Get the number of scenarios in the set.
        /*!
         * \return Number of scenarios.
         */
        uint32 getSize() const;

        //! Get the filename of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \return Filename.
         */
        const char* getFileName( uint32 scenario_id ) const;

        //! Get the name of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \return Name.
         */
        const char* getName( uint32 scenario_id ) const;

        //! Get the description of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \return Description.
         */
        const char* getDescription( uint32 scenario_id ) const;

        //! Get the unique identifier of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \return 64-bit identifier.
         */
        uint64 getUid( uint32 scenario_id ) const;

        //! Clear the set of scenarios.
        /*!
         */
        void clear();

        //! Remove a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         */
        void remove( uint32 scenario_id );

        //! Add the filename of a scenario.
        /*!
         * \param file_name Filename.
         */
        uint32 add( const char* file_name );

        //! Set the name of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \param name New name.
         */
        void setName( uint32 scenario_id, const char* name );

        //! Set the description of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \param descr New description.
         */
        void setDescription( uint32 scenario_id, const char* descr );

        //! Set the unique identifier of a given scenario.
        /*!
         * \param scenario_id Identifier of the scenario in the set.
         * \param uid New unique identifier.
         */
        void setUid( uint32 scenario_id, uint64 uid );


    public:
        std::vector<Ipseity::String> m_FileNames;
        std::vector<Ipseity::String> m_Names;
        std::vector<Ipseity::String> m_Descriptions;
		std::vector<uint64> m_Uids;
};


#endif
