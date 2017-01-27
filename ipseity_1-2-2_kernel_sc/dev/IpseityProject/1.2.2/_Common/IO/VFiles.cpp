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


//-----------------------------------------------------------------------------
// VFiles
//-----------------------------------------------------------------------------


#include "VFiles.h"

#include "MemDefns.h"
#include "StringFunctions.h"

#include <QDir>
#include <QByteArray>

#include <string.h>
#include <iostream>

using namespace std;


list<Ipseity::String> FileEnvironment::PathList;
Ipseity::String FileEnvironment::Path;
Ipseity::String FileEnvironment::MainPath;
Ipseity::String FileEnvironment::RelativePath;


//-----------------------------------------------------------------------------
// VFile
//-----------------------------------------------------------------------------

VFile::VFile()
{
    mem = NULL;
    ptr = NULL;
    size = 0;
}

VFile::VFile( const char* name, const char* base_path )
{
    load( name, base_path );
}

VFile::~VFile()
{
    SafeDeleteArray( mem );
}


bool
VFile::read( const char* file_name )
{
    QFile file( file_name );

    if (!file.open( QIODevice::ReadOnly ))
    {
        return false;
    }

//    cout << ">> Read " << file_name << endl;

    size = file.size();

    mem = new uint8 [size];
    file.read( (char*) mem, size );

    ptr = mem;

    file.close();

    return true;
}

bool
VFile::check( const char* search_path, const char* name )
{
    fname = search_path;
    fname += name;

    StringFunctions::replaceAntislashesBySlashes( fname.str() );

//	printf( "+ Searching %s\n", fname.c_str() );

    if (read( fname.c_str() ))
    {
//		cout << "Reading " << fname << endl;

        path = fname;
        path = path.getSubString( 0, path.findLastChar( '/' ) );

//		cout << "path " << path.c_str() << endl;

        return true;
    }
    else
    {
        return false;
    }
}


void
VFile::load( const char* name, const char* base_path )
{
    mem = NULL;
    ptr = NULL;
    size = 0;

//    if (strchr( base_path, ':' ))
//    {
//        Ipseity::String fname;

//        fname = base_path;
//        fname += name;

//        cout << ">> Read " << fname.c_str() << endl;

//        read( fname.c_str() );
//    }
//    else
    {
        FileEnvironment::init( base_path );

        if (strchr( name, ':' ))
        {
            read( name );
        }
        else
        {
            char _name[1024];

            strcpy( _name, name );

            StringFunctions::replaceAntislashesBySlashes( _name );

            // suppress initial '/'
            while( _name[0] == '/' && strlen(_name) > 0)
            {
                strcpy( _name, &(_name[1]) );
            }

            list<Ipseity::String>::iterator itr;

            for( itr = PathList.begin(); itr != PathList.end(); ++itr )
            {
                if (check( (*itr).c_str(), _name ))
                {
                    break;
                }
            }
        }
    }
}


bool
VFile::getString( char* buf, int bytes )
{
    if( ptr )
    {
        if( ptr+bytes > mem+size )
        {
            bytes = (mem+size)-ptr;
        }

        if( bytes > 0 )
        {
            int i = 0;

            while( i < bytes && (ptr[i] != '\r' && ptr[i] != '\n') )
            {
                buf[i] = ptr[i];
                i++;
            }

            if( ptr[i] == '\n' )
            {
                ptr += i+1;
            }
            else
            {
                ptr += i+2;
            }

            buf[i] = '\0';

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


//-----------------------------------------------------------------------------
// FileEnvironment
//-----------------------------------------------------------------------------


void
FileEnvironment::init( const char* base_path )
{
    MainPath = base_path;

    StringFunctions::replaceAntislashesBySlashes( MainPath.str() );

    if (MainPath[MainPath.getLength()-1] != '/' )
    {
        MainPath += "/";
    }

    PathList.clear();
    addPath( MainPath.c_str() );
    searchDirectory( MainPath.c_str() );
}


void
FileEnvironment::searchDirectory( const char* path )
{
    QDir directory( path );

    directory.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot );

    QFileInfoList list = directory.entryInfoList();
    QString dir;
    char* base_path;

    for( int32 i = 0; i < list.size(); ++i )
    {
        QFileInfo file = list.at( i );

        dir = file.filePath();
        dir += "/";
        base_path = (char*) dir.toStdString().c_str();
//		cout << "+ file: " << base_path << endl;

        addPath( base_path );
        searchDirectory( base_path );
    }
}


void
FileEnvironment::addPath( const char *path )
{
    PathList.push_back( path );
}


void
FileEnvironment::shut()
{
}
