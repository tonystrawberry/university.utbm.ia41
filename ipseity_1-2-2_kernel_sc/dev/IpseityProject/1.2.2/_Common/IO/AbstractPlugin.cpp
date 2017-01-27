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


#include "AbstractPlugin.h"


#include "MemDefns.h"

#include <iostream>

#include <QtGui>


using namespace std;


AbstractPlugin::AbstractPlugin() :
    m_Parent( NULL )
{
}


AbstractPlugin::~AbstractPlugin()
{
}


const std::map<Ipseity::String,Ipseity::String> &
AbstractPlugin::getInformation() const
{
    return m_Keywords;
}


const char*
AbstractPlugin::getInformation( const char* key ) const
{
    if (m_Keywords.find( key ) != m_Keywords.end())
    {
        return m_Keywords.at( key ).c_str();
    }
    else
    {
        return "-";
    }
}


void
AbstractPlugin::addInformation( const char* key, const char* information )
{
    m_Keywords[key] = information;
}


void
AbstractPlugin::setInstanceName( const char* instance_name )
{
    m_Name = getBaseName();

    if (instance_name)
    {
        m_Name += ".";
        m_Name += instance_name;
    }
}


void
AbstractPlugin::setParent( AbstractPlugin* parent )
{
    m_Parent = parent;

    if (parent)
    {
        Ipseity::String name;

        name = parent->getName();
        name += ".";
        name += m_Name;

        m_Name = name;
    }
}


void
AbstractPlugin::setVolatileVariables( const PluginVolatileVariables & vv )
{
    m_VolatileVariables =  vv;

    openSettings( m_VolatileVariables.m_Workspace.c_str() );
}


void
AbstractPlugin::setSearchRootPath( const char* path )
{
    m_VolatileVariables.m_SearchRootPath = path;
}


void
AbstractPlugin::setWorkspace( const char* workspace )
{
    m_VolatileVariables.m_Workspace = workspace;

    openSettings( workspace );
}
