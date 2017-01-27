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


#ifndef PluginWrapper_h
#define PluginWrapper_h


#include <QLibrary>

#include "AbstractPluginWrapper.h"
#include "AbstractPlugin.h"
#include "IpseityString.h"


#if defined( Q_WS_WIN )
  #define PLUGIN_EXTENSION ".dll"
#elif defined( Q_WS_X11 )
  #define PLUGIN_EXTENSION ".so"
#endif


template<class T>                   // T must inherit from AbstractPlugin
class PluginWrapper : public AbstractPluginWrapper
{
    public:
        PluginWrapper();
        ~PluginWrapper();

        void setParent( AbstractPlugin* );
        void setParameterFactory( ParameterRepository* parameter_factory );
        void setPluginSearchRootPath( const char* plugin_path );
        void setWorkspace( const char* workspace );

        void* get();

		bool check( const char* group_name, const char* name );

        bool load( const char* group_name, const char* name, const char* instance_name = NULL );

		bool free();

        const char* getGroupName() const;
        bool isGroupName( const char* ) const;

        const char* getPluginPath() const;

    protected:
        bool load( const char* name );
        bool searchForLibrary( const char* base_path, const char* name );
        bool loadLibrary( const char* name );

        bool freeLibrary();

    protected:
        AbstractPlugin* m_Parent;
        ParameterRepository* m_ParameterRepository;
        PluginVolatileVariables m_VolatileVariables;

    protected:
        T* m_Plugin;
        QLibrary m_Library;
        uint32 m_InstanceId;
        Ipseity::String m_Path;

        typedef T* (* lpFuncGetObject) ();
        lpFuncGetObject funcGetObject;

        typedef void (* lpFuncDestroy) ( uint32 );
        lpFuncDestroy funcDestroy;

        typedef const char* (* lpFuncGetString) ();
        lpFuncGetString funcGetGroupName;

        typedef uint32 (* lpFuncGetId) ();
        lpFuncGetId funcGetInstanceId;
};


#include "PluginWrapper.inl"


#endif
