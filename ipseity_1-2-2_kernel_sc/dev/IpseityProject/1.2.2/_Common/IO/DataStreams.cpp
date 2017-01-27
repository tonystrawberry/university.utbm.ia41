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

 
include "DataStreams.h"


DataStreams::DataStreams()
{
}

DataStreams::~DataStreams()
{
	clear();
}


uint32
DataStreams::getCount() const
{
	return m_StreamNames.size();
}


const char*
DataStreams::getName( uint32 i ) const
{
	return m_StreamNames[i].c_str();
}


const char*
DataStreams::getFileName( uint32 i ) const
{
	return m_FileNames[i].c_str();
}


void
DataStreams::clear()
{
	for( uint32 i = 0; i < getCount(); ++i )
	{
		if (m_Files[i]->is_open())
		{
			m_Files[i]->close();
			delete m_Files[i];
		}
	}
}


void
DataStreams::add( const char* stream_name )
{
	m_StreamNames.push_back( stream_name );
	m_Files.push_back( new std::ofstream );
	m_FileNames.push_back( "" );
}


void
DataStreams::open( const char* stream_name, const char* path, const char* fn )
{
	uint32 stream_id;

	if (queryId( stream_name, stream_id ))
	{
		m_FileNames[stream_id] = fn;

		if (m_Files[stream_id]->is_open())
		{
			m_Files[stream_id]->close();
		}

		if (!m_Files[stream_id]->good())
		{
			m_Files[stream_id]->clear();
		}

		char long_name[256];

		sprintf_s( long_name, "%s%s.txt", path, fn );
		m_Files[stream_id]->open( long_name, std::ios::out );
	}
}


bool
DataStreams::close( const char* stream_name, uint32 & stream_id )
{
	if (queryId( stream_name, stream_id ))
	{
		if (m_Files[stream_id]->is_open())
		{
			m_Files[stream_id]->close();
		}

		return true;
	}
	else
	{
		return false;
	}
}


std::ofstream* &
DataStreams::operator[]( uint32 i )
{
	return m_Files[i];
}


bool
DataStreams::queryId( const char* stream_name, uint32 & id )
{
	for( id = 0; id < getCount(); ++id )
	{
		if (!strcmp( m_StreamNames[id].c_str(), stream_name ))
		{
			return true;
		}
	}

	return false;
}
