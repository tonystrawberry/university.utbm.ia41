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


#ifndef IpseityString_h
#define IpseityString_h


#include "Types.h"

#include <ostream>


//! Simple implementation of a string.
/*!
 *  \author Fabrice LAURI
*/

namespace Ipseity
{

class String
{
    public:
        String();
        String( const String & );
        ~String();
        String & operator=( const String & );

        String & operator+=( const String & );
        String operator+( const String & ) const;

        String( const char* );
        String & operator=( const char* );
        String & operator+=( const char* );
        String operator+( const char* ) const;

        String & operator+=( uint32 );
        String operator+( uint32 ) const;

        String & operator+=( uint64 );
        String operator+( uint64 ) const;

        bool operator==( const String & ) const;
        bool operator!=( const String & ) const;
        bool operator<( const String & ) const;

		char & operator[]( uint32 i );

        const char* c_str() const;
		char* str();
        uint32 getLength() const;

        uint32 findFirstChar( char, bool = false ) const;
        uint32 findLastChar( char, bool = false ) const;

        bool find( const char* ) const;

        String getSubString( uint32 first ) const;
        String getSubString( uint32 first, uint32 size ) const;

        void replace( char, char );

        bool isInside( uint32 ) const;

        friend std::ostream & operator<<( std::ostream &, const String & );


    protected:
        uint32 m_Size;
        char* m_Content;
};

}


#endif
