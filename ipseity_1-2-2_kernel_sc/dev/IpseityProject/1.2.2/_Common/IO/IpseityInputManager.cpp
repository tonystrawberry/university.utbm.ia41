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


#include "IpseityInputManager.h"


#include <iostream>

using namespace std;


IpseityInputManager::IpseityInputManager() : InputManager()
{
}

IpseityInputManager::IpseityInputManager( uint32 nbr_keys ) : InputManager( nbr_keys )
{
}


void
IpseityInputManager::reset()
{
    InputManager::reset();

    for( uint32 i = 0; i < EP_NBR_COMMANDS; ++i )
    {
        m_NbrEvents[i] = 0;
        m_EventId[i].clear();
    }
    m_EventParameters.clear();
}


uint32
IpseityInputManager::getNbrEvents( uint32 command_id ) const
{
    if (command_id == 0)
    {
        return m_EventNames.size();
    }
    else
    {
        return m_NbrEvents[command_id-1];
    }
}


bool
IpseityInputManager::isEventOccurred( uint32 event_id, uint32 command_id )
{
    if (event_id < getNbrEvents( command_id ))
    {
        return InputManager::isEventOccurred( m_EventId[command_id-1][event_id] );
    }
    else
    {
        return false;
    }
}


const EventParameters &
IpseityInputManager::getEventParameters( uint32 event_id )
{
    return m_EventParameters[event_id];
}

const EventParameters &
IpseityInputManager::getEventParameters( uint32 event_id, uint32 command_id )
{
    return m_EventParameters[m_EventId[command_id-1][event_id]];
}


//*****************************************************************************


bool
IpseityInputManager::addEvent( const char* event_name, const char* keys, bool single, const EventParameters & event_parameters )
{
    if (event_parameters.CommandId && InputManager::addEvent( event_name, keys, single ))
    {
        uint32 event_id;

        event_id = m_EventParameters.size();

        m_EventParameters.push_back( event_parameters );

        uint32 command_id = event_parameters.CommandId-1;

        m_EventId[command_id].push_back( event_id );
        ++m_NbrEvents[command_id];

        return true;
    }
    else
    {
        return false;
    }
}


std::ostream &
operator<<( std::ostream & os, const IpseityInputManager & im )
{
    for( uint32 event_id = 0; event_id < im.m_Events.size(); ++event_id )
    {
        cout << event_id << ": ";
        for( uint32 i = 0; i < im.m_Events[event_id].size(); ++i )
        {
            if (i > 0)
            {
                os << "+";
            }
            os << im.m_Events[event_id].at( i );
        }
        if (im.m_SingleTypeEvents[event_id])
        {
            os << " (Single event) ";
        }
        os << " Command: " << im.m_EventParameters[event_id].CommandId;
        os << " Vector: " << im.m_EventParameters[event_id].Vector;
        os << endl;
    }

    return os;
}
