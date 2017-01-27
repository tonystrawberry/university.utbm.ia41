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


#ifndef StringFunctions_h
#define StringFunctions_h


#include "Types.h"

#include <vector>


//! Functions for managing strings.
/*!
 *  \author Fabrice LAURI
*/

class StringFunctions
{
    public:
        static void replaceSym1BySym2( char* buf, char sym1, char sym2 );
        static void replaceCommasByPoints( char * );
        static void replaceAntislashesBySlashes( char* );
        static void replaceSlashesByAntislashes( char* );

        static bool queryId( const char** strtab, const char* str, size_t size, uint32 & id );
        static bool queryId( std::vector<char*> strtab, const char* str, size_t size, uint32 & id );

        template <class O>
        static bool queryId( std::vector<O> strtab, const char* str, size_t size, uint32 & id );
};


template <class O>
bool
StringFunctions::queryId( std::vector<O> strtab, const char* str, size_t size, uint32 & i )
{
    i = 0;
    uint32 lenst = strlen( strtab[i]->getName() ), len;
    const char* ptr = str;

    // trim
    while( *ptr == ' ' && size > 1 )
    {
        ptr++;
        size--;
    }
    while( *(ptr+size-1) == ' ' && size > 1 )
    {
        size--;
    }

    while( i < strtab.size() )
    {
        if( lenst > size )
        {
            len = lenst;
        }
        else
        {
            len = size;
        }

        if( !strncmp( strtab[i]->getName(), ptr, len ) )
        {
            return true;
        }

        ++i;
        lenst = strlen( strtab[i]->getName() );
    }

    return false;
}


#endif
