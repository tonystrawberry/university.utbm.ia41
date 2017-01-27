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


#ifndef VFiles_h
#define VFiles_h


#include "Types.h"
#include "IpseityString.h"

#include <stdio.h>
#include <list>


//! Classes for loading files considered as resources.


//-----------------------------------------------------------------------------
//  FileEnvironment
//-----------------------------------------------------------------------------

class FileEnvironment
{
    public:
		static void init( const char* );
		static void shut();

	protected:
		static void addPath( const char* );
		static void searchDirectory( const char* );

	public:
        static std::list<Ipseity::String> PathList;
        static Ipseity::String Path;
        static Ipseity::String MainPath;
        static Ipseity::String RelativePath;
};


//-----------------------------------------------------------------------------
// VFile
//-----------------------------------------------------------------------------

class VFile : public FileEnvironment
{
    public:
        VFile();
        VFile( const char *name, const char* base_path );
        ~VFile();

        void load( const char *name, const char* base_path );

        bool check( const char*, const char* );

        bool getString( char* buf, int size );


	protected:
		bool read( const char* );


	public:
        Ipseity::String fname;
        Ipseity::String path;

		uint8* mem;
		uint32 size;

		uint8* ptr;
};


#endif
