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


#include "Scenarii.h"


uint32
Scenarii::getSize() const
{
    return m_Names.size();
}


const char*
Scenarii::getFileName( uint32 scenario_id ) const
{
	return m_FileNames[scenario_id].c_str();
}


const char*
Scenarii::getName( uint32 scenario_id ) const
{
	return m_Names[scenario_id].c_str();
}


const char*
Scenarii::getDescription( uint32 scenario_id ) const
{
	return m_Descriptions[scenario_id].c_str();
}


uint64
Scenarii::getUid( uint32 scenario_id ) const
{
	return m_Uids[scenario_id];
}


void
Scenarii::clear()
{
    m_FileNames.clear();
    m_Names.clear();
	m_Descriptions.clear();
	m_Uids.clear();
}


void
Scenarii::remove( uint32 scenario_id )
{
    if (scenario_id < getSize())
    {
		uint32 id = scenario_id;

        m_FileNames.erase( m_FileNames.begin()+id );
        m_Names.erase( m_Names.begin()+id );
		m_Descriptions.erase( m_Names.begin()+id );
		m_Uids.erase( m_Uids.begin()+id );
    }
}


uint32
Scenarii::add( const char* filename )
{
    uint32 id = m_Uids.size();

    Ipseity::String file_name;
    Ipseity::String name;
    file_name = filename;
    name = file_name.getSubString( file_name.findLastChar( '/' )+1, file_name.getLength() );
    name = name.getSubString( 0, name.findLastChar( '.' )-1 );

    m_FileNames.push_back( filename );
    m_Names.push_back( name );
	m_Descriptions.push_back( "" );
    m_Uids.push_back( 0 );

    return id;
}


void
Scenarii::setName( uint32 scenario_id, const char* name )
{
    if (scenario_id < getSize())
    {
		m_Names[scenario_id] = name;
    }
}


void
Scenarii::setDescription( uint32 scenario_id, const char* name )
{
	if (scenario_id < getSize())
	{
		m_Descriptions[scenario_id] = name;
	}
}


void
Scenarii::setUid( uint32 scenario_id, uint64 uid )
{
    if (scenario_id < getSize())
    {
		m_Uids[scenario_id] = uid;
    }
}
