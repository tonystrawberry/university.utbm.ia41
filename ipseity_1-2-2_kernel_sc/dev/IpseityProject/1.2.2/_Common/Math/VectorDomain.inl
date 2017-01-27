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

 
#include "VectorDomain.h"

#include "Math/RandomNumberGenerator.h"

#include <math.h>


template <class T>
VectorDomain<T>::VectorDomain() :
    m_Dimension( 0 ),
    m_Domains( NULL )
{
}

template <class T>
VectorDomain<T>::VectorDomain( const VectorDomain & av ) :
    m_Dimension( av.getDimension() )
{
    if (m_Dimension)
    {
        m_Domains = new ScalarDomain<T> [m_Dimension];
        for( int32 i = 0; i < m_Dimension; ++i )
        {
            m_Domains[i] = av.m_Domains[i];
        }
    }
    m_Cardinality = av.m_Cardinality;
}

template <class T>
VectorDomain<T>::~VectorDomain()
{
    clear();
}

template <class T>
VectorDomain<T> &
VectorDomain<T>::operator=( const VectorDomain & av )
{
    if( m_Dimension != av.m_Dimension )
    {
        SafeDeleteArray( m_Domains );
        m_Dimension = av.getDimension();
        if (m_Dimension)
        {
            m_Domains = new ScalarDomain<T> [m_Dimension];
        }
        else
        {
            m_Domains = NULL;
        }
    }

    if (m_Dimension)
    {
        for( int32 i = 0; i < m_Dimension; ++i )
        {
            m_Domains[i] = av.m_Domains[i];
        }
    }

    m_Cardinality = av.m_Cardinality;

    return *this;
}


template <class T>
VectorDomain<T>::VectorDomain( int32 size )
{
    m_Dimension = size;
    if (size)
    {
        m_Domains = new ScalarDomain<T> [size];
    }
    else
    {
        m_Domains = NULL;
    }
}


template <class T>
const ScalarDomain<T> &
VectorDomain<T>::operator[]( int32 i ) const
{
    return m_Domains[i];
}


template <class T>
int32
VectorDomain<T>::getDimension() const
{
    return m_Dimension;
}

template <class T>
void
VectorDomain<T>::setDimension( int32 dim )
{
    if (m_Dimension != dim)
    {
        SafeDeleteArray( m_Domains );
        m_Dimension = dim;
        if (dim)
        {
            m_Domains = new ScalarDomain<T> [dim];
        }
        else
        {
            m_Domains = NULL;
        }
        m_Cardinality.reset();
    }
}


template <class T>
void
VectorDomain<T>::setDomain( int32 index, const ScalarDomain<T> & domain )
{
    m_Domains[index] = domain;

    m_Cardinality.Class = CARDINALITY_FINITE;
    m_Cardinality.Count = 1;

    int32 i = 0;
    Cardinality card;

    while (i < m_Dimension && m_Cardinality.Class == CARDINALITY_FINITE)
    {
        card = m_Domains[i].getCardinality();

        if (card.Class == CARDINALITY_FINITE)
        {
            if (card.Count > 0)
            {
                m_Cardinality.Count *= card.Count;
            }
        }
        else
        {
            m_Cardinality.Class = CARDINALITY_INFINITE;
            m_Cardinality.Count = 0;
        }
        ++i;
    }
}


template <class T>
VectorDomain<T>
VectorDomain<T>::concat( const VectorDomain & av ) const
{
    int32 size = getDimension() + av.getDimension();
    VectorDomain rav( size );
    int32 k, i;

    k = 0;
    for( i = 0; i < getDimension(); ++i, ++k )
    {
        rav.setDomain( k, m_Domains[i] );
    }
    for( i = 0; i < av.getDimension(); ++i, ++k )
    {
        rav.setDomain( k, av.m_Domains[i] );
    }

    return rav;
}


template <class T>
Cardinality
VectorDomain<T>::getCardinality() const
{
    return m_Cardinality;
}


template <class T>
uint32
VectorDomain<T>::getIndex( const Vector<T> & v ) const
{
    if (m_Cardinality.Class == CARDINALITY_FINITE)
    {
        T nbr = (T) 0;
        T nbr_values;
        T min, max, interval;

        for( uint32 i = 0; i < m_Dimension; i++ )
        {
            min = m_Domains[i].getMin();
            max = m_Domains[i].getMax();
            interval = m_Domains[i].getInterval();

            nbr_values = (uint32) (1+((max-min)/interval));
            nbr *= nbr_values;
            nbr += (v[i]-min)/interval;
        }

        return (uint32) nbr;
    }
    else
    {
        cout << "Infinite space!!!" << endl;
        return 0;
    }
}


template <class T>
bool
VectorDomain<T>::query( const char* name, uint32 & id ) const
{
    for( id = 0; id < m_Dimension; id++ )
    {
        if( !strcmp( m_Domains[id].Name.c_str(), name ) )
        {
            return true;
        }
    }

    return false;
}


template <class T>
uint32
VectorDomain<T>::load( FILE* fp )
{
/*
    uint32 dim;

    fread( &dim, 1, sizeof( uint32 ), fp );
    setDimension( dim );

    for( int32 i = 0; i < m_Dimension; i++ )
    {
        fread( &m_Values[i], 1, sizeof( T ), fp );
    }

    bool bIntervals;
    fread( &bIntervals, 1, sizeof( bool ), fp );
    if (bIntervals)
    {
        SafeDeleteArray( m_Mins );
        m_Mins = new T [m_Dimension];
        SafeDeleteArray( m_Maxs );
        m_Maxs = new T [m_Dimension];
        for( int32 i = 0; i < m_Dimension; i++ )
        {
            fread( &m_Mins[i], 1, sizeof( T ), fp );
            fread( &m_Maxs[i], 1, sizeof( T ), fp );
        }
    }

    bool bShifts;
    fread( &bShifts, 1, sizeof( bool ), fp );
    if (bShifts)
    {
        SafeDeleteArray( m_Shifts );
        m_Shifts = new T [m_Dimension];
        for( int32 i = 0; i < m_Dimension; i++ )
        {
            fread( &m_Shifts[i], 1, sizeof( T ), fp );
        }
    }
*/
    return 1;
}

template <class T>
uint32
VectorDomain<T>::save( FILE* fp ) const
{
/*
    fwrite( &m_Dimension, 1, sizeof( uint32 ), fp );

    for( int32 i = 0; i < m_Dimension; i++ )
    {
        fwrite( &m_Values[i], 1, sizeof( T ), fp );
    }

    bool bIntervals = m_Mins ? true : false;
    fwrite( &bIntervals, 1, sizeof( bool ), fp );
    if (bIntervals)
    {
        for( int32 i = 0; i < m_Dimension; i++ )
        {
            fwrite( &m_Mins[i], 1, sizeof( T ), fp );
            fwrite( &m_Maxs[i], 1, sizeof( T ), fp );
        }
    }

    bool bShifts = m_Shifts ? true : false;
    fwrite( &bShifts, 1, sizeof( bool ), fp );
    if (bShifts)
    {
        for( int32 i = 0; i < m_Dimension; i++ )
        {
            fwrite( &m_Shifts[i], 1, sizeof( T ), fp );
        }
    }
*/
    return 1;
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const VectorDomain<T> & domain )
{
    for( int32 i = 0; i < domain.getDimension(); ++i )
    {
        os << i << ": ";
        os << domain[i];
        os << endl;
    }

    if (domain.m_Cardinality.Class == CARDINALITY_FINITE)
    {
        if (domain.m_Cardinality.Count == 0)
        {
            os << "Empty space!" << endl;
        }
        else
        {
            os << "Finite space - Cardinality: " << domain.m_Cardinality.Count << endl;
        }
    }
    else
    {
        os << "Infinite space" << endl;
    }

    return os;
}


template <class T>
QDebug
operator<<( QDebug os, const VectorDomain<T> & domain )
{
    for( int32 i = 0; i < domain.getDimension(); ++i )
    {
        os.nospace() << i << ": ";
        os.nospace() << domain[i];
        os.nospace() << endl;
    }

    if (domain.m_Cardinality.Class == CARDINALITY_FINITE)
    {
        if (domain.m_Cardinality.Count == 0)
        {
            os.nospace() << "Empty space!" << endl;
        }
        else
        {
            os.nospace() << "Finite space - Cardinality: " << domain.m_Cardinality.Count << endl;
        }
    }
    else
    {
        os.nospace() << "Infinite space" << endl;
    }

    return os.maybeSpace();
}


template <class T>
void
VectorDomain<T>::clear()
{
    m_Dimension = 0;
    SafeDeleteArray( m_Domains );
    m_Cardinality.reset();
}
