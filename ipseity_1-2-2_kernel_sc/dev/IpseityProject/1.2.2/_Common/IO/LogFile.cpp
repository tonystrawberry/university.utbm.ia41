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


#include "LogFile.h"

#include <stdarg.h>


LogFile::LogFile( char* name )
{
	m_Fp = fopen( name, "w" );
	m_ScreenBackUp = false;
}

LogFile::~LogFile()
{
	if( m_Fp )
	{
		fflush( m_Fp );
		fclose( m_Fp );
	}
}


void LogFile::backUpToScreen( bool f )
{
	m_ScreenBackUp = true;
}


void LogFile::out( char* format, ... )
{
	if( m_Fp )
	{
		char buffer[256];

		va_list args;
		va_start( args, format );
		vsprintf( buffer, format, args );
		fprintf( m_Fp, buffer );
		fflush( m_Fp );
		if( m_ScreenBackUp )
		{
			printf( buffer );
		}
		va_end( args );
	}
}


void LogFile::outln( char* format, ... )
{
	if( m_Fp )
	{
		char buffer[256];

		va_list args;
		va_start( args, format );
		vsprintf( buffer, format, args );
		fprintf( m_Fp, buffer );
		fflush( m_Fp );
		fprintf( m_Fp, "\n" );
		fflush( m_Fp );
		if( m_ScreenBackUp )
		{
			printf( buffer );
			printf( "\n" );
		}
		va_end( args );
	}
}

void LogFile::outln()
{
	if( m_Fp )
	{
		fprintf( m_Fp, "\n" );
		fflush( m_Fp );
	}
}


void LogFile::outsep()
{
	if( m_Fp )
	{
		fprintf( m_Fp, "------------------------------------------------------------------\n" );
		fflush( m_Fp );
	}
}
