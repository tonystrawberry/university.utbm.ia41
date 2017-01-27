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


#include "Stimulus.h"

#include "Types.h"


//! Implementation of a stimulus perceived by an agent.
/*!
    A stimulus is represented by a tuple of sensory channels.
    Each sensory channel is a vector whose components are of type \a IpseityPrecisionType.
    \author Fabrice LAURI
*/


Stimulus Stimulus::VoidStimulus;


Stimulus::Stimulus() :
    m_Status( 0 ),
    m_ChannelId( 0 ),
    m_Dimension( 0 ),
    m_Stimulus( NULL )
{
}

Stimulus::Stimulus( const Stimulus & s )
{
    m_Status = s.m_Status;
    m_ChannelId = s.m_ChannelId;
    m_Dimension = s.m_Dimension;
    m_Stimulus = new SensorySignal [m_Dimension];
    for( uint32 i = 0; i < s.m_Dimension; ++i )
    {
        m_Stimulus[i] = s.m_Stimulus[i];
    }
}

Stimulus &
Stimulus::operator=( const Stimulus & s )
{
    m_Status = s.m_Status;
    m_Dimension = s.m_Dimension;
    m_ChannelId = s.m_ChannelId;
    SafeDeleteArray( m_Stimulus );
    m_Stimulus = new SensorySignal [m_Dimension];
    for( uint32 i = 0; i < s.m_Dimension; ++i )
    {
        m_Stimulus[i] = s.m_Stimulus[i];
    }

    return (*this);
}

Stimulus::~Stimulus()
{
    SafeDeleteArray( m_Stimulus );
}


bool
Stimulus::operator==( const Stimulus & s ) const
{
    if (m_Dimension != s.m_Dimension)
    {
        return false;
    }

    for( uint32 i = 0; i < s.m_Dimension; ++i )
    {
        if (m_Stimulus[i] != s.m_Stimulus[i])
        {
            return false;
        }
    }

    return true;
}


uint32
Stimulus::getStatus() const
{
    return m_Status;
}

bool
Stimulus::isTerminal() const
{
    return Terminal;
}

bool
Stimulus::isAGoal() const
{
    return Goal;
}


void
Stimulus::setStatus( uint32 s )
{
    m_Status = s;
}

void
Stimulus::setTerminal( bool f )
{
    Terminal = f;
}

void
Stimulus::setGoal( bool f )
{
    Goal = f;
}


uint32
Stimulus::getNbrSensoryChannels() const
{
    return m_Dimension;
}

void
Stimulus::setNbrSensoryChannels( uint32 n )
{
    SafeDeleteArray( m_Stimulus );
    m_Dimension = n;
    m_Stimulus = new SensorySignal [n];
    m_ChannelId = 0;
}


void
Stimulus::selectSensoryChannel( uint32 index )
{
	if (index < m_Dimension)
	{
		m_ChannelId = index;
	}
}


void
Stimulus::setDimension( int32 dim )
{
    m_Stimulus[m_ChannelId].setDimension( dim );
    m_Stimulus[m_ChannelId].fill( 0.0 );
}


Ipseity::Scalar &
Stimulus::operator[]( int32 i )
{
    return m_Stimulus[m_ChannelId][i];
}


const SensorySignal &
Stimulus::getCurrentSensoryData() const
{
    return m_Stimulus[m_ChannelId];
}

void
Stimulus::setCurrentSensoryData( const SensorySignal & s )
{
    m_Stimulus[m_ChannelId] = s;
}


const SensorySignal &
Stimulus::getSensoryData( uint32 channel_id ) const
{
    return m_Stimulus[channel_id];
}

void
Stimulus::setSensoryData( uint32 channel_id, const SensorySignal & s )
{
    m_Stimulus[channel_id] = s;
}


std::ostream &
operator<<( std::ostream & os, const Stimulus & s )
{
    for( uint32 i = 0; i < s.m_Dimension; ++i )
    {
//        os << i << ": " << "(" << s.m_Stimulus[i] << ") ";
        os << s.m_Stimulus[i] << " ";
    }

    return os;
}


QDebug
operator<<( QDebug os, const Stimulus & s )
{
    for( uint32 i = 0; i < s.m_Dimension; ++i )
    {
        os.nospace() << "(" << s.m_Stimulus[i] << ") ";
    }

    return os.maybeSpace();
}
