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


#ifndef IpseityInputManager_h
#define IpseityInputManager_h


#include "IO/InputManager.h"
#include "AL/Response.h"


enum
{
    EP_COMMAND_SUGGEST_RESPONSE	= 1,
    EP_COMMAND_GENERATE_REWARD,
    EP_COMMAND_SENDTO_ENVIRONMENT,
    EP_COMMAND_CHANGE_CS_STATUS
};

const uint32 EP_NBR_COMMANDS = 4;


class EventParameters
{
    public:
        uint32 CommandId;
        Response Vector;
};


class IpseityInputManager : public InputManager
{
    public:
        IpseityInputManager();
        IpseityInputManager( uint32 nbr_keys );

        void reset();

        uint32 getNbrEvents( uint32 command_id ) const;

        bool isEventOccurred( uint32 event_id, uint32 command_id );

        const EventParameters & getEventParameters( uint32 event_id );
        const EventParameters & getEventParameters( uint32 event_id, uint32 command_id );

        bool addEvent( const char* event_name, const char* keys, bool single, const EventParameters & );

        friend std::ostream & operator<<( std::ostream &, const IpseityInputManager & );


    protected:
        uint32 m_NbrEvents[EP_NBR_COMMANDS];
        std::vector<uint32> m_EventId[EP_NBR_COMMANDS];
        std::vector<EventParameters> m_EventParameters;
};


#endif
