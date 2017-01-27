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


#include "StaticLookupTable.h"


StaticLookupTable::StaticLookupTable() :
    m_NbrCells( 0 ),
    m_Values( NULL ),
    m_Eligibilities( NULL )
{
}


StaticLookupTable::~StaticLookupTable()
{
    deleteWidgets();

    SafeDeleteArray( m_Values );
    SafeDeleteArray( m_Eligibilities );
}


const char*
StaticLookupTable::getBaseName() const
{
    return "StaticLookupTable";
}


const char*
StaticLookupTable::getDescription() const
{
    return "";
}


const char*
StaticLookupTable::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
StaticLookupTable::defineParameters()
{
}


bool
StaticLookupTable::attachView( uint32 view_id, QWidget* )
{
    return false;
}


void
StaticLookupTable::deleteWidgets()
{
}


bool
StaticLookupTable::isDynamic() const
{
    return false;
}


void
StaticLookupTable::init()
{
    Cardinality cardinality = m_Description.getCardinality();
    if (cardinality.Class == CARDINALITY_FINITE)
    {
        m_NbrCells = cardinality.Count;
    }

    SafeDeleteArray( m_Values );
    SafeDeleteArray( m_Eligibilities );

    m_Values = new Ipseity::Scalar [m_NbrCells];
    m_Eligibilities = new Ipseity::Scalar [m_NbrCells];
//	setNbrAlphas( m_NbrCells );

    reset();
}


Ipseity::Scalar
StaticLookupTable::getPercentageVisitedCells()
{
    return 0.0;
}


void
StaticLookupTable::updateLabelPercentageVisitedCells()
{
}


bool
StaticLookupTable::canReset() const
{
    return true;
}


void
StaticLookupTable::reset()
{
    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        m_Values[i] = 0.0;
        m_Eligibilities[i] = 0.0;
    }
//    resetAlphas();
}


uint32
StaticLookupTable::getNbrCells() const
{
    return m_NbrCells;
}


Ipseity::Scalar
StaticLookupTable::getValue( const SensorySignal & s, const Response & a )
{
    uint32 i;
    SensorySignal sa;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        i = m_Indices.size();
        m_Indices[sa] = i;
    }
    else
    {
        i = m_Indices[sa];
    }

    if (i < m_NbrCells)
    {
        return m_Values[i];
    }
    else
    {
//		cout << "getValue: Cell id out of range!" << endl;

        return 0.0;
    }
}


uint32
StaticLookupTable::getNbrVisits( const SensorySignal & x, const Response & u )
{
    return 0;
}


void
StaticLookupTable::update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate )
{
    Ipseity::Scalar td ;

    td = new_estimate-old_estimate;

    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        m_Values[i] += getAlpha( i, td )*td*m_Eligibilities[i];
    }
}


void
StaticLookupTable::clearEligibilityTraces()
{
    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        m_Eligibilities[i] = 0.0;
    }
}


void
StaticLookupTable::decayEligibilityTraces( Ipseity::Scalar factor )
{
    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        m_Eligibilities[i] *= factor;
    }
}


void
StaticLookupTable::replaceEligibilityTraces( const SensorySignal & s, const Response & a )
{
    uint32 i;
    SensorySignal sa;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        i = m_Indices.size();
        m_Indices[sa] = i;
    }
    else
    {
        i = m_Indices[sa];
    }

    if (i < m_NbrCells)
    {
        m_Eligibilities[i] = 1.0;

//        updateAlpha( i );
    }
    else
    {
//		cout << "replaceEligibilityTraces: Cell id out of range!" << endl;
    }
}


void
StaticLookupTable::accumulateEligibilityTraces( const SensorySignal & s, const Response & a )
{
    uint32 i;
    SensorySignal sa;

    sa = s.concat( a );

    if (m_Indices.find( sa ) == m_Indices.end())
    {
        i = m_Indices.size();
        m_Indices[sa] = i;
    }
    else
    {
        i = m_Indices[sa];
    }

    if (i < m_NbrCells)
    {
        m_Eligibilities[i] += 1.0;

//        updateAlpha( i );
    }
    else
    {
//		cout << "accumulateEligibilityTraces: Cell id out of range!" << endl;
    }
}


bool
StaticLookupTable::load( FILE* file )
{
    uint32 nbr;

    fread( &nbr, sizeof( uint32 ), 1, file );
    if (nbr == m_NbrCells)
    {
        Ipseity::Scalar value;
        Ipseity::Scalar eligibility;

        for( uint32 i = 0; i < nbr; ++i )
        {
            fread( &value, sizeof( Ipseity::Scalar ), 1, file );
            fread( &eligibility, sizeof( Ipseity::Scalar ), 1, file );

            m_Values[i] = value;
            m_Eligibilities[i] = eligibility;
        }

        Vector<Ipseity::Scalar> v;
        uint32 index;

        fread( &nbr, sizeof( uint32 ), 1, file );
        for( uint32 i = 0; i < nbr; ++i )
        {
            v.load( file );
            fread( &index, sizeof( uint32 ), 1, file );

            m_Indices[v] = index;
        }

        return true;
    }
    else
    {
        cout << "error reading mem" << endl;

        return false;
    }
}


bool
StaticLookupTable::save( FILE* file )
{
    fwrite( &m_NbrCells, sizeof( uint32 ), 1, file );
    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        fwrite( &m_Values[i], sizeof( Ipseity::Scalar ), 1, file );
        fwrite( &m_Eligibilities[i], sizeof( Ipseity::Scalar ), 1, file );
    }

    TIndices::const_iterator itr_indices;
    uint32 nbr;

    nbr = m_Indices.size();
    fwrite( &nbr, sizeof( uint32 ), 1, file );
    for( itr_indices = m_Indices.begin(); itr_indices != m_Indices.end(); ++itr_indices )
    {
        (*itr_indices).first.save( file );
        fwrite( &(*itr_indices).second, sizeof( uint32 ), 1, file );
    }

    return true;
}
