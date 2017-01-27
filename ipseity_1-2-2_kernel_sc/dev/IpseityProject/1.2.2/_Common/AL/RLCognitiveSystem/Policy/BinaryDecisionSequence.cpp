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


#include "BinaryDecisionSequence.h"


BinaryDecisionSequence::BinaryDecisionSequence()
{
	m_Size = 0;
	m_Dimension = 0;
	m_Decisions = NULL;
	m_TemporaryResponses = NULL;
}

BinaryDecisionSequence::BinaryDecisionSequence( const BinaryDecisionSequence & s )
{
	m_Size = s.m_Size;
	m_Dimension = s.m_Dimension;

	if (m_Size)
	{
		m_Decisions = new Response [m_Size];
		m_TemporaryResponses = new Response [m_Size];
		for( uint32 i = 0; i < m_Size; ++i )
		{
			m_Decisions[i] = s.m_Decisions[i];
			m_TemporaryResponses[i] = s.m_TemporaryResponses[i];
		}
	}
	else
	{
		m_Decisions = NULL;
		m_TemporaryResponses = NULL;
	}
}

BinaryDecisionSequence &
BinaryDecisionSequence::operator=( const BinaryDecisionSequence & s )
{
	if (m_Size != s.m_Size)
	{
		SafeDeleteArray( m_Decisions );
		SafeDeleteArray( m_TemporaryResponses );

		m_Size = s.m_Size;

		if (m_Size)
		{
			m_Decisions = new Response [m_Size];
			m_TemporaryResponses = new Response [m_Size];
		}
	}

	m_Dimension = s.m_Dimension;
	for( uint32 i = 0; i < m_Size; ++i )
	{
		m_Decisions[i] = s.m_Decisions[i];
		m_TemporaryResponses[i] = s.m_TemporaryResponses[i];
	}

	return (*this);
}

BinaryDecisionSequence::~BinaryDecisionSequence()
{
	SafeDeleteArray( m_Decisions );
	SafeDeleteArray( m_TemporaryResponses );
}


void
BinaryDecisionSequence::setSize( uint32 size, uint32 dimension )
{
	if (m_Size != size)
	{
		SafeDeleteArray( m_Decisions );
		SafeDeleteArray( m_TemporaryResponses );

		m_Size = size;

		m_Decisions = new Response [size];
		m_TemporaryResponses = new Response [size];
	}

	m_Dimension = dimension;
	for( uint32 i = 0; i < size; ++i )
	{
		m_Decisions[i].setDimension( dimension );
		m_TemporaryResponses[i].setDimension( dimension );
	}
}


const Response &
BinaryDecisionSequence::getDecision( uint32 id ) const
{
	return m_Decisions[id];
}

void
BinaryDecisionSequence::setDecision( uint32 id, const Response & r )
{
	m_Decisions[id] = r;
}


const Response &
BinaryDecisionSequence::getTemporaryResponse( uint32 id ) const
{
	return m_TemporaryResponses[id];
}

void
BinaryDecisionSequence::setTemporaryResponse( uint32 id, const Response & r )
{
	m_TemporaryResponses[id] = r;
}
