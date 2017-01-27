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


#include "StringFunctions.h"

#include <string.h>
#include <stdio.h>


void
StringFunctions::replaceSym1BySym2( char* buf, char sym1, char sym2 )
{
    char* ptr = buf;

    while( *ptr )
    {
        if( *ptr == sym1 )
        {
            *ptr = sym2;
        }

        ++ptr;
    }
}


void
StringFunctions::replaceCommasByPoints( char* buf )
{
    replaceSym1BySym2( buf, ',', '.' );
}


void
StringFunctions::replaceAntislashesBySlashes( char* buf )
{
    replaceSym1BySym2( buf, '\\', '/' );
}


void
StringFunctions::replaceSlashesByAntislashes( char* buf )
{
    replaceSym1BySym2( buf, '/', '\\' );
}


bool
StringFunctions::queryId( const char** strtab, const char* str, size_t size, uint32 & i )
{
    const char* ptr = str;

    // trim
    while( *ptr == ' ' )
    {
        ++ptr;
        --size;
    }
    while( *(ptr+size-1) == ' ' )
    {
        --size;
    }

    i = 0;
    uint32 lenst = strlen( strtab[i] );
    while( lenst > 0 )
    {
        if (lenst == size && !strncmp( strtab[i], ptr, size ) )
        {
            return true;
        }

        ++i;
        lenst = strlen( strtab[i] );
    }

    return false;
}


bool
StringFunctions::queryId( std::vector<char*> strtab, const char* str, size_t size, uint32 & i )
{
    const char* ptr = str;

    // trim
    while( *ptr == ' ' && size > 1 )
    {
        ++ptr;
        --size;
    }
    while( *(ptr+size-1) == ' ' && size > 1 )
    {
        --size;
    }

    i = 0;
    while( i < strtab.size() )
    {
        if (strlen( strtab[i] ) == size && !strncmp( strtab[i], ptr, size ))
        {
            return true;
        }

        ++i;
    }

    return false;
}
