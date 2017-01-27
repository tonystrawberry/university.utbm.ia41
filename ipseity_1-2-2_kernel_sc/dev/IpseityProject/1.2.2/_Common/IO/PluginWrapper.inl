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


#include "PluginWrapper.h"

#include "Types.h"

#include <QDir>
#include <QDebug>


template<class T>
PluginWrapper<T>::PluginWrapper() :
    m_Parent( NULL ),
    m_ParameterRepository( NULL ),
    m_Plugin( NULL ),
    m_InstanceId( 0 )
{
}

template<class T>
PluginWrapper<T>::~PluginWrapper()
{
    free();
}


template<class T>
void
PluginWrapper<T>::setParent( AbstractPlugin* parent )
{
    m_Parent = parent;
    m_ParameterRepository = parent->getParameterRepository();
    m_VolatileVariables = parent->m_VolatileVariables;
}


template<class T>
void
PluginWrapper<T>::setParameterFactory( ParameterRepository* parameter_factory )
{
    m_ParameterRepository = parameter_factory;
}


template<class T>
void
PluginWrapper<T>::setPluginSearchRootPath( const char* search_root_path )
{
    m_VolatileVariables.m_SearchRootPath = search_root_path;
}


template<class T>
void
PluginWrapper<T>::setWorkspace( const char* workspace )
{
    m_VolatileVariables.m_Workspace = workspace;
}


template<class T>
void*
PluginWrapper<T>::get()
{
    return reinterpret_cast<void*>(m_Plugin);
}


template<class T>
bool
PluginWrapper<T>::check( const char* group_name, const char* name )
{
    free();

    if (load( name ))
    {
        if (isGroupName( group_name ))
        {
            qDebug() << "Plugin '" << name << "' in group '" << group_name << "' exists!";

            funcGetObject = (lpFuncGetObject) m_Library.resolve( "GetPlugin" );
            if (funcGetObject)
            {
                m_Plugin = funcGetObject();
                m_InstanceId = funcGetInstanceId();

                return true;
            }
        }
    }

    free();
    return false;
}


template<class T>
bool
PluginWrapper<T>::load( const char* group_name, const char* name, const char* instance_name )
{
    free();

    if (strlen( name ) && load( name ))
    {
        if (isGroupName( group_name ))
        {
            qDebug() << "Plugin '" << name << "' in group '" << group_name << "' has been found!";

            funcGetObject = (lpFuncGetObject) m_Library.resolve( "GetPlugin" );
            if (funcGetObject)
            {
                m_Plugin = funcGetObject();
                m_InstanceId = funcGetInstanceId();

                m_Plugin->setInstanceName( instance_name );
                m_Plugin->setParameterRepository( m_ParameterRepository );
                m_Plugin->setParent( m_Parent );
                m_Plugin->defineParameters();
                m_Plugin->setVolatileVariables( m_VolatileVariables );

                qDebug() << "*****>>  Plugin '" << m_Plugin->getName() << "' loaded!";

                return true;
            }
        }
    }

    free();
    return false;
}


template<class T>
bool
PluginWrapper<T>::free()
{
    if (m_Plugin)
    {
//        qDebug() << ">>> Free plugin" << m_Name.c_str();

        funcDestroy( m_InstanceId );
        m_Plugin = NULL;
    }

    return freeLibrary();
}


template<class T>
const char*
PluginWrapper<T>::getGroupName() const
{
    if (funcGetGroupName)
    {
        return funcGetGroupName();
    }
    else
    {
        return "Unknown Group";
    }
}


template<class T>
bool
PluginWrapper<T>::isGroupName( const char* name ) const
{
    return !strcmp( getGroupName(), name );
}


template<class T>
const char*
PluginWrapper<T>::getPluginPath() const
{
    return m_Path.c_str();
}


//*****************************************************************************


template<class T>
bool
PluginWrapper<T>::load( const char* name )
{
    if (searchForLibrary( m_VolatileVariables.m_SearchRootPath.c_str(), name ))
    {
        funcDestroy = (lpFuncDestroy) m_Library.resolve( "DestroyPlugin" );
        funcGetGroupName = (lpFuncGetString) m_Library.resolve( "GetPluginGroupName" );
        funcGetInstanceId = (lpFuncGetId) m_Library.resolve( "GetInstanceId" );     // Does not work with distributed plugins!

        if (funcDestroy && funcGetGroupName && funcGetInstanceId)
        {
            return true;
        }
    }

    return false;
}


template<class T>
bool
PluginWrapper<T>::searchForLibrary( const char* base_path, const char* name )
{
    QDir directory( base_path );

    directory.setFilter( QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot );
    directory.setSorting( QDir::DirsLast );

    QFileInfoList list = directory.entryInfoList();

//        cout << "files: " << list.size() << endl;

    for( int32 i = 0; i < list.size(); ++i )
    {
        QFileInfo file = list.at( i );

//            cout << "+ file: " << file.absoluteFilePath().toStdString().c_str() << endl;

        if (file.isDir())
        {
//                cout << "++ file: " << dir.toStdString().c_str() << endl;

            if (searchForLibrary( file.absoluteFilePath().toStdString().c_str(), name ))
            {
                return true;
            }
        }
        else //if (strstr( file.fileName().toStdString().c_str(), PLUGIN_EXTENSION ))
        {
            QString baseName;

            baseName = name;
            baseName = baseName.left(baseName.lastIndexOf(PLUGIN_EXTENSION));

            if (!strcmp( file.baseName().toStdString().c_str(), baseName.toStdString().c_str() )
                  && loadLibrary( file.absoluteFilePath().toStdString().c_str() ))
            {
                m_Path = file.absolutePath().toStdString().c_str();
                m_Path += "/";

                return true;
            }
        }
    }

    return false;
}


template<class T>
bool
PluginWrapper<T>::loadLibrary( const char* name )
{
    m_Library.setFileName( name );
    m_Library.load();

    return m_Library.isLoaded();
}


template<class T>
bool
PluginWrapper<T>::freeLibrary()
{
    return m_Library.unload();
}
