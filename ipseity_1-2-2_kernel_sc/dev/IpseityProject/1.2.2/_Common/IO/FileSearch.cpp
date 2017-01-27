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


#include "FileSearch.h"

#include <QDir>

#include <iostream>

using namespace std;


bool
FileSearch::lookForFiles( const char* base_path, uint32 status, bool (*pn)( const char* ), const char* extension, int32 max_depth )
{
    QDir directory( base_path );

    directory.setFilter( QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot );
    directory.setSorting( QDir::DirsLast );

    QFileInfoList list = directory.entryInfoList();
    bool ret_value = false;

//	cout << "# files: " << list.size() << endl;

    for( int32 i = 0; i < list.size(); ++i )
    {
        QFileInfo file = list.at( i );

//		cout << "+ file: " << file.filePath().toStdString().c_str() << "  /  " << file.fileName().toStdString().c_str() << endl;

        if (file.isDir())
        {
            if (max_depth != 0)
            {
                if ((status & FS_CALL_FUNCTION_FOR_DIRECTORY) && pn)
                {
                    if (status & FS_USE_LONGNAMES)
                    {
//					std::cout << "*Pn dir1: " << file.filePath().toStdString().c_str() << "(" << max_depth << ")" << std::endl;
                        ret_value = pn( file.filePath().toStdString().c_str() );
                    }
                    else
                    {
//					std::cout << "*Pn dir2: " << file.fileName().toStdString().c_str() << "(" << max_depth << ")" << std::endl;
                        ret_value = pn( file.fileName().toStdString().c_str() );
                    }
                }

                if (status & FS_RECURSIVE_PATHS)
                {
                    int32 depth = max_depth;

                    if (depth > 0)
                    {
                        --depth;
                    }

//					std::cout << "*Goto dir: " << file.filePath().toStdString().c_str() << "(" << max_depth << ")" << std::endl;
                    ret_value = lookForFiles( file.filePath().toStdString().c_str(), status, pn, extension, depth );
                }
            }
        }
        else if (!(status & FS_CALL_FUNCTION_FOR_DIRECTORY))
        {
            if (strstr( file.fileName().toStdString().c_str(), extension ) && pn )
            {
                if (status & FS_USE_LONGNAMES)
                {
//					std::cout << "*Pn file1: " << file.filePath().toStdString().c_str() << "(" << max_depth << ")" << std::endl;
                    ret_value = pn( file.filePath().toStdString().c_str() );
                }
                else
                {
//					std::cout << "*Pn file2: " << file.fileName().toStdString().c_str() << "(" << max_depth << ")" << std::endl;
                    ret_value = pn( file.fileName().toStdString().c_str() );
                }

                if (!(status & FS_ITERATE_FILES))
                {
                    break;
                }
            }
        }
    }

    return ret_value;
}


void
FileSearch::removeFilesRecursively( const char* path, const char* exclude_ext, bool delete_empty_dir )
{
    QDir directory( path );

    directory.setFilter( QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot );
    directory.setSorting( QDir::DirsLast );

    QFileInfoList list = directory.entryInfoList();

    for( int32 i = 0; i < list.size(); ++i )
    {
        QFileInfo file = list.at( i );

        if (file.isDir())
        {
            removeFilesRecursively( file.filePath().toStdString().c_str(), exclude_ext, delete_empty_dir );

            if (delete_empty_dir)
            {
                directory.rmdir( file.fileName().toStdString().c_str() );
            }
        }
        else if (file.isFile() && (strlen( exclude_ext ) == 0 || !strstr( file.fileName().toStdString().c_str(), exclude_ext )))
        {
//            std::cout << "*file: " << file.fileName().toStdString().c_str() << std::endl;
            directory.remove( file.fileName() );
        }
    }
}
