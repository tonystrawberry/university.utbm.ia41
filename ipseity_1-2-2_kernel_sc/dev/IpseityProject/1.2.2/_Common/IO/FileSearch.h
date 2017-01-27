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


#ifndef FileSearch_h
#define FileSearch_h


#include "Types.h"


enum
{
    FS_RECURSIVE_PATHS				= 1,
    FS_ITERATE_FILES				= 2,
    FS_USE_LONGNAMES				= 4,
    FS_CALL_FUNCTION_FOR_DIRECTORY	= 8
};


class FileSearch
{
    public:
        static bool lookForFiles( const char* base_path, uint32 status, bool (*pn)( const char* ), const char* extension = "", int32 max_depth = -1 );

        static void removeFilesRecursively( const char* path, const char* exclude_ext, bool delete_empty_dir );
};


#endif
