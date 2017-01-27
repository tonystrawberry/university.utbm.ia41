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


#include "IO/ParameterRepository.h"


#include "IO/AbstractControl.h"
#include "IO/AbstractParameter.h"


#include <iostream>

using namespace std;



void
ParameterRepository::addControl( AbstractControl* control )
{
    m_Controls.push_back( control );
}

void
ParameterRepository::removeControl( AbstractControl* control )
{
    m_Controls.remove( control );
}

void
ParameterRepository::displayControls()
{
    list<AbstractControl*>::const_iterator itr;
    uint32 control_id;

    cout << "--------------------------------------" << endl;
    cout << "Controls:" << endl;
    for( control_id = 1, itr = m_Controls.begin(); itr != m_Controls.end(); ++control_id, ++itr )
    {
        cout << " " << control_id << "- " << (*itr)->getName();
        cout << " (" << (*itr)->getHostName() << ")";
        cout << endl;
    }
    cout << "--------------------------------------" << endl;
}


void
ParameterRepository::addParameter( AbstractParameter* parameter )
{
    m_Parameters.push_back( parameter );
}

void
ParameterRepository::removeParameter( AbstractParameter* parameter )
{
    m_Parameters.remove( parameter );
}

AbstractParameter*
ParameterRepository::findParameter( const char* parameter_name )
{
    list<AbstractParameter*>::const_iterator itr;
    uint32 i;

    for( i = 1, itr = m_Parameters.begin(); itr != m_Parameters.end(); ++i, ++itr )
    {
        if (!strcmp( (*itr)->getName(), parameter_name ))
        {
//            cout << i << ": " << name << " in " << (*itr)->getHostName() << endl;
            return (*itr);
        }
    }

    return NULL;
}

void
ParameterRepository::displayParameters()
{
    list<AbstractParameter*>::const_iterator itr;
    uint32 parameter_id;

    cout << "--------------------------------------" << endl;
    cout << "Parameters:" << endl;
    for( parameter_id = 1, itr = m_Parameters.begin(); itr != m_Parameters.end(); ++parameter_id, ++itr )
    {
        cout << " " << parameter_id << "- " << (*itr)->getName();
        cout << ": " << (*itr)->toString();
        cout << " (" << (*itr)->getHostName() << ")";
        cout << endl;
    }
    cout << "--------------------------------------" << endl;
}
