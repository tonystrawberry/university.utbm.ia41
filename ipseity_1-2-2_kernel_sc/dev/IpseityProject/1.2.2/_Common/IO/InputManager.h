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


#ifndef InputManager_h
#define InputManager_h


#include "Types.h"
#include "IpseityString.h"
#include "IO/AbstractInputManager.h"
#include "Math/Vector.h"

#include <vector>
#include <map>
#include <ostream>


//! Input manager.
/*!
 * \brief The InputManager class
 */

class InputManager : public AbstractInputManager
{
    typedef std::vector<uint32> KeyCombinationType;

    public:
        InputManager();
        InputManager( uint32 nbr_keys );
        ~InputManager();

        virtual void reset();

        void pressKey( uint32 );
        void releaseKey( uint32 );

        uint32 getNbrEvents() const;

        bool isEventOccurred( uint32 event_id );

        const char* getEventName( uint32 event_id ) const;

        bool addEvent( const char* event_name, const char* keys, bool single );

        friend std::ostream & operator<<( std::ostream &, const InputManager & );


    protected:
        void init( uint32 nbr_keys );

        bool isKeyPressed( uint32 );
        bool isKeyPressedOnce( uint32 );
        uint32 getNbrPressedKeys() const;

        uint32 getKeyCode( const char* key_name, uint32 key_name_size = 0 );

        void computeEventOccurrences();

        void convertKey( uint32 & );


    protected:
        uint32 m_NbrKeys;
        bool* m_PressedKeys;
        bool* m_QueriedKeys;
        uint32 m_NbrPressedKeys;

        std::vector<Ipseity::String> m_EventNames;
        std::vector<KeyCombinationType> m_Events;
        std::vector<bool> m_SingleTypeEvents;
        std::vector<bool> m_OccurredEvents;
};


#endif
