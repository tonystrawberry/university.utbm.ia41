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


#include "AbstractParameter.h"

#include "MemDefns.h"
#include "ParameterHost.h"


ParameterHost* AbstractParameter::g_Host = NULL;


AbstractParameter::AbstractParameter( const char* name ) :
    m_Name( name ),
    m_ValueString( NULL ),
    m_Settings( NULL )
{
    m_Host = g_Host;
    if (m_Host)
    {
        m_Host->addParameter( this );
    }
}


AbstractParameter::~AbstractParameter()
{
    SafeDelete( m_ValueString );
    if (m_Host)
    {
        m_Host->removeParameter( this );
    }
}


const char*
AbstractParameter::getName() const
{
    return m_Name.c_str();
}


const char*
AbstractParameter::getHostName() const
{
    if (m_Host)
    {
        return m_Host->getName();
    }
    else
    {
        return "Unknown";
    }
}

//*****************************************************************************


void
AbstractParameter::init( QSettings* settings )
{
    m_Settings = settings;
    if (settings)
    {
        if (settings->contains( m_Name.c_str() ))
        {
            initValue();
        }
    }
}


void
AbstractParameter::save()
{
    saveValue();
}
