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


#ifndef AbstractPluginWrapper_h
#define AbstractPluginWrapper_h


class AbstractPlugin;


class AbstractPluginWrapper
{
    public:
        virtual void setParent( AbstractPlugin* ) = 0;
        virtual void setPluginSearchRootPath( const char* plugin_path ) = 0;
        virtual void setWorkspace( const char* workspace ) = 0;

        virtual void* get() = 0;

        virtual bool check( const char* group_name, const char* name ) = 0;

        virtual bool load( const char* group_name, const char* name, const char* instance_name ) = 0;

        virtual bool free() = 0;

        virtual const char* getGroupName() const = 0;
        virtual bool isGroupName( const char* ) const = 0;

        virtual const char* getPluginPath() const = 0;
};


#endif
