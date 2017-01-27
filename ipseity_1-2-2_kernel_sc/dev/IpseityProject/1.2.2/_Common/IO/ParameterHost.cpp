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


#include "ParameterHost.h"


#include "IO/AbstractControl.h"
#include "IO/AbstractParameter.h"
#include "IO/ParameterRepository.h"

#include <QSettings>


ParameterHost::ParameterHost() :
    m_ParameterRepository( NULL ),
    m_Settings( NULL )
{
    AbstractParameter::g_Host = this;
    AbstractControl::g_Host = this;
}


ParameterHost::~ParameterHost()
{
    if (m_Settings)
    {
        m_Settings->endGroup();

        cout << "Save settings of " << m_Settings->fileName().toStdString().c_str() << endl;

        delete m_Settings;
    }
}


const char*
ParameterHost::getName() const
{
    return m_Name.c_str();
}


ParameterRepository*
ParameterHost::getParameterRepository() const
{
    return m_ParameterRepository;
}


void
ParameterHost::setParameterRepository( ParameterRepository* parameter_factory )
{
    m_ParameterRepository = parameter_factory;
}


void
ParameterHost::addControl( AbstractControl* control )
{
    if (m_ParameterRepository)
    {
        m_ParameterRepository->addControl( control );
    }
}

void
ParameterHost::removeControl( AbstractControl* control )
{
    if (m_ParameterRepository)
    {
        m_ParameterRepository->removeControl( control );
    }
}


void
ParameterHost::addParameter( AbstractParameter* parameter )
{
    m_Parameters.push_front( parameter );
    if (m_ParameterRepository)
    {
        m_ParameterRepository->addParameter( parameter );
    }
}

void
ParameterHost::removeParameter( AbstractParameter* parameter )
{
    m_Parameters.remove( parameter );
    if (m_ParameterRepository)
    {
        m_ParameterRepository->removeParameter( parameter );
    }
}

AbstractParameter*
ParameterHost::findParameter( const char* parameter_name )
{
    if (m_ParameterRepository)
    {
        return m_ParameterRepository->findParameter( parameter_name );
    }
    else
    {
        return NULL;
    }
}


void
ParameterHost::closeSettings()
{
    if (m_Settings)
    {
        for( std::list<AbstractParameter*>::iterator itr = m_Parameters.begin(); itr != m_Parameters.end(); ++itr )
        {
            (*itr)->save();
        }
        m_Settings->endGroup();
        SafeDelete( m_Settings );

        initParametersSettings();
    }
}


//*****************************************************************************


void
ParameterHost::openSettings( const char* workspace_path )
{
    Ipseity::String file_name;

    file_name = workspace_path;
    file_name += "settings.ini";

    SafeDelete( m_Settings );
    m_Settings = new QSettings( file_name.c_str(), QSettings::IniFormat );
    qDebug() << "Settings of " << getName() << " will be saved in file" << file_name.c_str();
    m_Settings->beginGroup( getName() );

    initParametersSettings();
}


void
ParameterHost::initParametersSettings()
{
    // Init the settings file for all the attributes
    for( std::list<AbstractParameter*>::iterator itr = m_Parameters.begin(); itr != m_Parameters.end(); ++itr )
    {
        (*itr)->init( m_Settings );
    }
}


void
ParameterHost::clearParameters()
{
    std::list<AbstractParameter*>::iterator itr;

    while( (itr = m_Parameters.begin()) != m_Parameters.end() )
    {
//        cout << "Delete " << (*itr)->getName() << " ";
//        cout << (*itr)->getHostName() << endl;
        delete (*itr);
    }

//    system( "pause" );
}
