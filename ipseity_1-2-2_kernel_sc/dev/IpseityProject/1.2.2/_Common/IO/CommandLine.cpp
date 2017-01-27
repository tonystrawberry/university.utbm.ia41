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


#include "CommandLine.h"

#include <stdio.h>


void getCommandLine( LPSTR cline, int & argc, char** & argv )
{
	char* ptr = cline;
	char* space_ptr;
	char* quote_ptr1;
	char* quote_ptr2;
	int nc;
	int i;

	argc = 0;
	while( *ptr != '\0' )
	{
		quote_ptr1 = strchr( ptr, '\"' );
		space_ptr = strchr( ptr, ' ' );
		if( space_ptr )
		{
			quote_ptr2 = strchr( space_ptr, '\"' );
		}

		if( space_ptr )
		{
			if( quote_ptr1 == NULL || space_ptr < quote_ptr1 && space_ptr > quote_ptr2 )
			{
				++argc;
				ptr = space_ptr+1;

				while( *ptr == ' ' )
				{
					++ptr;
				}
			}
		}
		else
		{
			++argc;
			break;
		}
	}

	argv = new char* [argc];

	ptr = cline;
	for( i = 0; i < argc; i++ )
	{
		if( space_ptr = strchr( ptr, ' ' ) )
		{
			nc = space_ptr-ptr;
		}
		else
		{
			nc = strlen( ptr );
		}

		argv[i] = new char [nc+1];
		strncpy( argv[i], ptr, nc );
		argv[i][nc] = '\0';

		ptr += nc+1;
		while( *ptr == ' ' )
		{
			ptr++;
		}
	}
}
