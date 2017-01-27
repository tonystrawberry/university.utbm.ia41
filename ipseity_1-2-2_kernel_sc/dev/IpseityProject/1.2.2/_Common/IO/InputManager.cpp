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


#include "InputManager.h"


#include "ScanCodes.h"
#include "MemDefns.h"

#include <stdio.h>
#include <iostream>

using namespace std;


const uint32 cstMaxKeys = 1024;


InputManager::InputManager()
{
    init( cstMaxKeys );
}

InputManager::InputManager( uint32 nbr_keys )
{
    init( nbr_keys );
}

InputManager::~InputManager()
{
    SafeDeleteArray( m_PressedKeys );
    SafeDeleteArray( m_QueriedKeys );
}


void
InputManager::init( uint32 nbr_keys )
{
    m_NbrKeys = nbr_keys;
    m_PressedKeys = new bool [m_NbrKeys];
    m_QueriedKeys = new bool [m_NbrKeys];
}


bool
InputManager::isKeyPressed( uint32 key_code )
{
    if (key_code < m_NbrKeys)
    {
        return m_PressedKeys[key_code];
    }
    else
    {
        return false;
    }
}

bool
InputManager::isKeyPressedOnce( uint32 key_code )
{
    bool pressed = false;

    if (key_code < m_NbrKeys)
    {
        if (m_PressedKeys[key_code] && m_QueriedKeys[key_code])
        {
            m_QueriedKeys[key_code] = false;

            pressed = true;
        }
    }

//    cout << key_code << ": " << pressed << endl;

    return pressed;
}

uint32
InputManager::getNbrPressedKeys() const
{
    return m_NbrPressedKeys;
}


uint32
InputManager::getKeyCode( const char* key_name, uint32 key_name_size )
{
    uint32 i;

    i = 0;
    if (key_name_size == 0)
    {
        while( strcmp( KeyNames[i].Name, "" ) && strcmp( KeyNames[i].Name, key_name ) )
        {
            ++i;
        }
    }
    else
    {
        while( strcmp( KeyNames[i].Name, "" ) &&
                (strlen( KeyNames[i].Name ) != key_name_size ||
                strncmp( KeyNames[i].Name, key_name, key_name_size )))
        {
            ++i;
        }
    }

//	cout << "*" << key_name_size << " " << KeyNames[i].Name << "*" << endl;

    return KeyNames[i].Code;
}


void
InputManager::reset()
{
    memset( m_PressedKeys, 0, m_NbrKeys*sizeof( bool ) );
    memset( m_QueriedKeys, 0, m_NbrKeys*sizeof( bool ) );
    m_NbrPressedKeys = 0;

    m_EventNames.clear();
    m_Events.clear();
    m_SingleTypeEvents.clear();
    m_OccurredEvents.clear();
}


void
InputManager::pressKey( uint32 key_code )
{
    convertKey( key_code );

    if (key_code < m_NbrKeys)
    {
        if (!m_PressedKeys[key_code])
        {
            ++m_NbrPressedKeys;
            m_QueriedKeys[key_code] = true;
        }
        m_PressedKeys[key_code] = true;

//        cout << "Press Key: " << key_code << "(" << m_QueriedKeys[key_code] << ")" << endl;

        computeEventOccurrences();
    }
}

void
InputManager::releaseKey( uint32 key_code )
{
    convertKey( key_code );

    if (key_code < m_NbrKeys)
    {
        if (m_PressedKeys[key_code])
        {
            --m_NbrPressedKeys;
            m_QueriedKeys[key_code] = false;
        }
        m_PressedKeys[key_code] = false;

//        cout << "Release Key: " << key_code << "(" << m_QueriedKeys[key_code] << ")" << endl;

        computeEventOccurrences();
    }
}


uint32
InputManager::getNbrEvents() const
{
    return m_EventNames.size();
}


bool
InputManager::isEventOccurred( uint32 event_id )
{
    if (event_id < getNbrEvents())
    {
        if (m_SingleTypeEvents[event_id])
        {
            KeyCombinationType* kc;
            KeyCombinationType::const_iterator itr;
            bool event_occurred;

            kc = &m_Events[event_id];

            event_occurred = true;
            for( itr = kc->begin(); itr != kc->end(); ++itr )
            {
                if (!isKeyPressedOnce( (*itr) ))
                {
                    event_occurred = false;
                    break;
                }
            }

            return event_occurred;
        }
        else
        {
            return m_OccurredEvents[event_id];
        }
    }
    else
    {
        return false;
    }
}


//*****************************************************************************


bool
InputManager::addEvent( const char* event_name, const char* keys, bool single )
{
    KeyCombinationType key_combination;
    const char* ptr;
    const char* next_ptr;
    uint32 key_code;
    uint32 size;

    ptr = keys;
    do
    {
        next_ptr = strchr( ptr, '+' );

        if (next_ptr)
        {
            size = next_ptr-ptr;
        }
        else
        {
            size = strlen( ptr );
        }
        key_code = getKeyCode( ptr, size );
        key_combination.push_back( key_code );

        if (next_ptr)
        {
            ptr = next_ptr+1;
        }
    }
    while (next_ptr);

    if (key_combination.size())
    {
        Ipseity::String name;
        uint32 event_id;

        name = event_name;
        event_id = m_EventNames.size();

        m_EventNames.push_back( name );
        m_Events.push_back( key_combination );
        m_SingleTypeEvents.push_back( single );
        m_OccurredEvents.push_back( false );

        return true;
    }
    else
    {
        return false;
    }
}


const char*
InputManager::getEventName( uint32 event_id ) const
{
    return m_EventNames[event_id].c_str();
}


std::ostream &
operator<<( std::ostream & os, const InputManager & im )
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
        os << endl;
    }

    return os;
}


void
InputManager::computeEventOccurrences()
{
    KeyCombinationType* kc;
    KeyCombinationType::const_iterator itr;
    bool event_occurred;

    for( uint32 event_id = 0; event_id < m_EventNames.size(); ++event_id )
    {
        event_occurred = true;
//        cout << "Event: " << event_id << endl;
        if (!m_SingleTypeEvents[event_id])
        {
            kc = &m_Events[event_id];
            for( itr = kc->begin(); itr != kc->end(); ++itr )
            {
                if (!isKeyPressed( (*itr) ))
                {
                    event_occurred = false;
                    break;
                }
            }
        }

        m_OccurredEvents[event_id] = event_occurred;
    }
}


void
InputManager::convertKey( uint32 & key_code )
{
    if (key_code >= 0x1000000)
    {
        key_code -= 0xFFFF00;
    }
}
