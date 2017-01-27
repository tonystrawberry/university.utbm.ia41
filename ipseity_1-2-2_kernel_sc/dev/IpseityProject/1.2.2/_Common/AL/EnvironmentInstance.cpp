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


#include "EnvironmentInstance.h"


EnvironmentInstance::EnvironmentInstance()
{
    SensoryInterface si0;

    si0.add( "Percepts" );

    m_SensoryInterfaces.push_back( si0 );

    m_Pixmap = QPixmap( ":/Icon.png" );
}


const QPixmap &
EnvironmentInstance::getPixmap() const
{
    return m_Pixmap;
}


void
EnvironmentInstance::setParentEnvironment( AbstractEnvironment* env )
{
	m_ParentEnvironment = env;
}


uint32
EnvironmentInstance::getNbrTaxons() const
{
	return m_Taxons.size();
}


const Taxon &
EnvironmentInstance::getTaxon( uint32 taxon_id )
{
	return m_Taxons[taxon_id];
}


uint32
EnvironmentInstance::getNbrSensoryInterfaces() const
{
    return m_SensoryInterfaces.size();
}


const SensoryInterface &
EnvironmentInstance::getSensoryInterface( uint32 sensory_interface_id )
{
    return m_SensoryInterfaces[sensory_interface_id];
}


//*****************************************************************************


void
EnvironmentInstance::resizeWindow( uint32 window_id, int32 width, int32 height )
{
	m_Width.resize( window_id+1 );
	m_Height.resize( window_id+1 );

    m_Width[window_id] = width;
    m_Height[window_id] = height;
}


//*****************************************************************************


uint32
EnvironmentInstance::addTaxon( const char* name, uint32 status, uint32 sensory_interface_id )
{
	Taxon taxon;
    uint32 taxon_id = m_Taxons.size();

    taxon.Id = taxon_id;
	taxon.Name = name;
	taxon.Status = status;
    taxon.SensoryInterfaceId = sensory_interface_id;

	m_Taxons.push_back( taxon );

    return taxon_id;
}
