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


#include "DynamicLookupTable.h"


DynamicLookupTable::DynamicLookupTable() :
    m_Layout( NULL )
{
}


DynamicLookupTable::~DynamicLookupTable()
{
    deleteWidgets();
}


const char*
DynamicLookupTable::getBaseName() const
{
    return "DynamicLookupTable";
}


const char*
DynamicLookupTable::getDescription() const
{
    return "";
}


const char*
DynamicLookupTable::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
DynamicLookupTable::defineParameters()
{
    m_Layout = new QGridLayout();
    m_Label = new QLabel();
    m_Layout->addWidget( m_Label, 1,1,1,1 );
}

bool
DynamicLookupTable::attachView( uint32 view_id, QWidget* widget )
{
    updateLabelPercentageVisitedCells();

    widget->setLayout( m_Layout );

    return true;
}

void
DynamicLookupTable::deleteWidgets()
{
	if (m_Layout)
	{
		delete m_Label;

        delete m_Layout;
	}
}


bool
DynamicLookupTable::isDynamic() const
{
    return true;
}


void
DynamicLookupTable::init()
{
}


Ipseity::Scalar
DynamicLookupTable::getPercentageVisitedCells()
{
    return 100.0;
}


void
DynamicLookupTable::updateLabelPercentageVisitedCells()
{
    m_Label->setText( QString( "Visited cells: %1" ).arg( m_Indices.size() ) );
}


bool
DynamicLookupTable::canReset() const
{
    return true;
}


void
DynamicLookupTable::reset()
{
    m_Indices.clear();
    m_Values.clear();
    m_Eligibilities.clear();

	updateLabelPercentageVisitedCells();
}


uint32
DynamicLookupTable::getNbrCells() const
{
    return m_Values.size();
}


Ipseity::Scalar
DynamicLookupTable::getValue( const SensorySignal & s, const Response & a )
{
    SensorySignal sa;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        m_Indices[sa] = m_Indices.size();

        m_Values.push_back( 0.0 );
        m_Eligibilities.push_back( 0.0 );
//        insertOneAlpha();

        return 0.0;
    }
    else
    {
        return m_Values[m_Indices[sa]];
    }
}


uint32
DynamicLookupTable::getNbrVisits( const SensorySignal & x, const Response & u )
{
    return 0;
}


void
DynamicLookupTable::update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate )
{
    Ipseity::Scalar new_value;
    Ipseity::Scalar td ;
    uint32 index;

    td = new_estimate-old_estimate;

    for( uint32 i = 0; i < m_ActivatedCells.size(); ++i )
    {
        index = m_ActivatedCells[i];
		new_value = m_Values[index] + getAlpha( index, td )*td*m_Eligibilities[index];

//		if (m_Values[index] == 0.0 || new_value > m_Values[index])
        {
            m_Values[index] = new_value;
        }
    }

	updateLabelPercentageVisitedCells();
}


void
DynamicLookupTable::clearEligibilityTraces()
{
    uint32 index;

    for( uint32 i = 0; i < m_ActivatedCells.size(); ++i )
    {
        index = m_ActivatedCells[i];
        m_Eligibilities[index] = 0.0;
    }
    m_ActivatedCells.clear();
}


void
DynamicLookupTable::decayEligibilityTraces( Ipseity::Scalar factor )
{
    uint32 index;

    for( uint32 i = 0; i < m_ActivatedCells.size(); ++i )
    {
        index = m_ActivatedCells[i];
        m_Eligibilities[index] *= factor;
    }
}


void
DynamicLookupTable::replaceEligibilityTraces( const SensorySignal & s, const Response & a )
{
    SensorySignal sa;
    uint32 i;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        i = m_Indices.size();
        m_Indices[sa] = i;

        m_Values.push_back( 0.0 );
        m_Eligibilities.push_back( 1.0 );
//        insertOneAlpha();
    }
    else
    {
        i = m_Indices[sa];
        m_Eligibilities[i] = 1.0;
    }

    m_ActivatedCells.push_back( i );
//    updateAlpha( i );
}


void
DynamicLookupTable::accumulateEligibilityTraces( const SensorySignal & s, const Response & a )
{
    SensorySignal sa;
    uint32 i;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        i = m_Indices.size();
        m_Indices[sa] = i;

        m_Values.push_back( 0.0 );
        m_Eligibilities.push_back( 1.0 );
//        insertOneAlpha();
    }
    else
    {
        i = m_Indices[sa];
        m_Eligibilities[i] += 1.0;
    }

    m_ActivatedCells.push_back( i );
//    updateAlpha( i );
}


bool
DynamicLookupTable::load( FILE* file )
{
    uint32 nbr;
	Vector<Ipseity::Scalar> v;
    uint32 index;

    fread( &nbr, sizeof( uint32 ), 1, file );
    for( uint32 i = 0; i < nbr; ++i )
    {
        v.load( file );
        fread( &index, sizeof( uint32 ), 1, file );

        m_Indices[v] = index;
    }

    Ipseity::Scalar value;
    Ipseity::Scalar eligibility;

    fread( &nbr, sizeof( uint32 ), 1, file );
    for( uint32 i = 0; i < nbr; ++i )
    {
        fread( &value, sizeof( Ipseity::Scalar ), 1, file );
        fread( &eligibility, sizeof( Ipseity::Scalar ), 1, file );

        m_Values.push_back( value );
        m_Eligibilities.push_back( eligibility );
    }

    return true;
}


bool
DynamicLookupTable::save( FILE* file )
{
    TIndices::const_iterator itr_indices;
    uint32 nbr;

    nbr = m_Indices.size();
    fwrite( &nbr, sizeof( uint32 ), 1, file );
    for( itr_indices = m_Indices.begin(); itr_indices != m_Indices.end(); ++itr_indices )
    {
        (*itr_indices).first.save( file );
        fwrite( &(*itr_indices).second, sizeof( uint32 ), 1, file );
    }

    nbr = m_Values.size();
    fwrite( &nbr, sizeof( uint32 ), 1, file );
    for( uint32 i = 0; i < nbr; ++i )
    {
        fwrite( &m_Values[i], sizeof( Ipseity::Scalar ), 1, file );
        fwrite( &m_Eligibilities[i], sizeof( Ipseity::Scalar ), 1, file );
    }

    return true;
}
