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

 
#include "XMLFile.h"


#include <stdio.h>
#include <string.h>
#include <iostream>


#ifdef WIN32
  #include <direct.h>         // for _getcwd
  #include <tchar.h>          // for _vsntprintf
  #include <stdarg.h>         // for va_start, va_end
#else
  #include <stdlib.h>
  #include <stdarg.h>         // for va_start, va_end
  #include <unistd.h>         // for getcwd
  #define _getcwd getcwd
  #define _vsntprintf vsnprintf
#endif


using namespace std;


XMLFile::XMLFile()
{
    _MaxNr = 0;
}

XMLFile::~XMLFile()
{
    list<XMLLine>::iterator itr;

    for( itr = _Strings.begin(); itr != _Strings.end(); itr++ )
    {
            delete [] (*itr).content;
    }
}


void XMLFile::clear()
{
    _Strings.clear();
    _MaxNr = 0;
}


void XMLFile::add( uint32 nr, const char * s, ... )
{
    XMLLine line;

    va_list msg;
    char buffer[LINELENGTH] = { '\0' };
    char * output;

    va_start( msg, s );
    _vsntprintf( buffer, LINELENGTH - 1, s, msg );
    va_end( msg );

    output = new char [strlen( buffer )+1];
    strcpy( output, buffer );

    line.nr = nr;
    line.content = output;

    _Strings.push_back( line );

    if (nr > _MaxNr)
    {
        _MaxNr = nr;
    }
}


void XMLFile::save( const char* fname )
{
    list<XMLLine>::iterator itr;
    FILE * fp;
    uint32 nb;

    std::cout << "Saving " << fname << std::endl;
    fp = fopen( fname, "w" );

    if (fp)
    {
        for( uint32 i = 0; i <= _MaxNr; i++ )
        {
            nb = 0;
            for( itr = _Strings.begin(); itr != _Strings.end(); itr++ )
            {
                if( (*itr).nr == i )
                {
                    fprintf( fp, "%s", (*itr).content );
                    nb++;
                }
            }

            if (nb)
            {
                fprintf( fp, "\n" );
            }
        }

        fclose( fp );
    }
}
