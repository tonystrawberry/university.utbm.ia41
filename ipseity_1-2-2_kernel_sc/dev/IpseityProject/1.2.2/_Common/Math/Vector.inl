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

 
#include "Vector.h"


#include "MemDefns.h"
#include "Math/RandomNumberGenerator.h"

#include <math.h>

#include <QDebug>


template <class T>
Vector<T>::Vector() :
    m_Dimension( 0 ),
    m_Stored( false ),
    m_Values( NULL )
{
}

template <class T>
Vector<T>::Vector( const Vector & av )
{
    m_Dimension = av.getDimension();
    if (m_Dimension)
    {
        m_Stored = true;
        m_Values = new T [m_Dimension];
        memcpy( m_Values, av.m_Values, m_Dimension*sizeof( T ) );
    }
    else
    {
        m_Stored = false;
        m_Values = NULL;
    }
}

template <class T>
Vector<T>::~Vector()
{
    clear();
}

template <class T>
Vector<T> &
Vector<T>::operator=( const Vector & av )
{
    if( m_Dimension != av.m_Dimension )
    {
        if (m_Stored)
        {
            SafeDeleteArray( m_Values );
        }
        m_Dimension = av.getDimension();
        if (m_Dimension)
        {
            m_Stored = true;
            m_Values = new T [m_Dimension];
        }
        else
        {
            m_Stored = false;
            m_Values = NULL;
        }
    }
    if (m_Dimension)
    {
        memcpy( m_Values, av.m_Values, m_Dimension*sizeof( T ) );
    }

    return *this;
}


template <class T>
Vector<T> &
Vector<T>::operator=( T* array )
{
    if (!m_Stored)
    {
        m_Values = array;
    }

    return *this;
}


template <class T>
Vector<T>::Vector( int32 size ) :
    m_Dimension( size )
{
    if (size)
    {
        m_Stored = true;
        m_Values = new T [size];
    }
    else
    {
        m_Stored = false;
        m_Values = NULL;
    }
}


template <class T>
void
Vector<T>::clear()
{
    m_Dimension = 0;
    if (m_Stored)
    {
        SafeDeleteArray( m_Values );
    }
}


template <class T>
T &
Vector<T>::operator[]( int32 i ) const
{
    if (i < 0)
    {
        qCritical() << "Vector component access: beyond lower bound (" << i << ")!";
        return m_Values[0];
    }
    else if (i >= m_Dimension)
    {
        qCritical() << "Vector component access: beyond higher bound (" << i << ")!";
        return m_Values[m_Dimension-1];
    }
    else
    {
        return m_Values[i];
    }
}


template <class T>
int32
Vector<T>::getDimension() const
{
    return m_Dimension;
}


template <class T>
void
Vector<T>::setDimension( int32 dim, bool stored )
{
    if (m_Dimension != dim)
    {
        if (m_Stored)
        {
            SafeDeleteArray( m_Values );
        }
        m_Dimension = dim;
        if (dim)
        {
            if (stored)
            {
                m_Stored = true;
                m_Values = new T [dim];
            }
            else
            {
                m_Stored = false;
            }
        }
        else
        {
            m_Stored = false;
            m_Values = NULL;
        }
    }
}


template <class T>
bool
Vector<T>::operator==( const Vector<T> & av ) const
{
    if (getDimension() == av.getDimension())
    {
        for( int32 i = 0; i < getDimension(); i++ )
        {
            if (m_Values[i] != av.m_Values[i])
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}


template <class T>
bool
Vector<T>::operator!=( const Vector & av ) const
{
    return !this->operator==( av );
}


template <class T>
bool
Vector<T>::operator<( const Vector & av ) const
{
    if (getDimension() == av.getDimension())
    {
        if (getDimension() > 0)
        {
            int32 i = 0, n = getDimension()-1;

            while( i < n && m_Values[i] == av.m_Values[i] )
            {
                i++;
            }

            return (m_Values[i] < av.m_Values[i]);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return (getDimension() < av.getDimension());
    }
}


template <class T>
Vector<T> &
Vector<T>::operator+=( const Vector & av )
{
    if( m_Dimension == av.m_Dimension )
    {
        for( uint32 i = 0; i < m_Dimension; ++i )
        {
            m_Values[i] += av[i];
        }
    }

    return *this;
}


template <class T>
Vector<T> &
Vector<T>::operator*=( const Vector & av )
{
    if (m_Dimension == av.m_Dimension)
    {
        for( uint32 i = 0; i < m_Dimension; ++i )
        {
            m_Values[i] *= av[i];
        }
    }

    return *this;
}


template <class T>
Vector<T> &
Vector<T>::operator*=( const T & f )
{
    for( uint32 i = 0; i < m_Dimension; ++i )
    {
        m_Values[i] *= f;
    }

    return *this;
}


template <class T>
Vector<T>
Vector<T>::operator*( const Vector & av ) const
{
    Vector v( *this );

    if (m_Dimension == av.m_Dimension)
    {
        for( uint32 i = 0; i < m_Dimension; ++i )
        {
            v[i] *= av[i];
        }
    }

    return v;
}


template <class T>
Vector<T>
Vector<T>::operator/( const Vector & av ) const
{
    Vector v( *this );

    if (m_Dimension == av.m_Dimension)
    {
        for( int32 i = 0; i < getDimension(); i++ )
        {
            v[i] /= av[i];
        }
    }

    return v;
}


template <class T>
Vector<T>
Vector<T>::operator*( const T & f ) const
{
    Vector v( *this );

    for( uint32 i = 0; i < m_Dimension; ++i )
    {
        v[i] *= f;
    }

    return v;
}


template <class T>
Vector<T>
Vector<T>::concat( const Vector & av ) const
{
    int32 size = getDimension() + av.getDimension();
    Vector rav( size );

    memcpy( rav.m_Values, m_Values, getDimension()*sizeof( T ) );
    memcpy( rav.m_Values+getDimension(), av.m_Values, av.getDimension()*sizeof( T ) );

    return rav;
}


template <class T>
void
Vector<T>::fill( T v )
{
    for( int32 i = 0; i < getDimension(); i++ )
    {
        m_Values[i] = v;
    }
}


template <class T>
void
Vector<T>::setToMinVector( VectorDomain<T>* properties )
{
    if (properties)
    {
        setDimension( properties->getDimension() );
        for( int32 i = 0; i < getDimension(); i++ )
        {
            m_Values[i] = (*properties)[i].getMin();
        }
    }
}


template <class T>
void
Vector<T>::setToMaxVector( VectorDomain<T>* properties )
{
    if (properties)
    {
        setDimension( properties->getDimension() );
        for( int32 i = 0; i < getDimension(); i++ )
        {
            m_Values[i] = (*properties)[i].Max;
        }
    }
}


template <class T>
void
Vector<T>::setRandomVector( VectorDomain<T>* properties )
{
    if (properties)
    {
        setDimension( properties->getDimension() );

        int32 n;
        T min, max, interval;

        for( int32 i = 0; i < getDimension(); i++ )
        {
            min = (*properties)[i].getMin();
            max = (*properties)[i].getMax();
            interval = (*properties)[i].getInterval();

            if (interval > 0)
            {
                n = 1+((int32) (max-min)/interval);
                m_Values[i] = min+RandomNumberGenerator::getRandomNumber( n )*interval;
            }
            else
            {
                m_Values[i] = RandomNumberGenerator::getRandomNumber( min, max );
            }
        }
    }
}


template <class T>
bool
Vector<T>::inc( VectorDomain<T>* properties )
{
    if (properties)
    {
        int32 nbr = getDimension();

        for( int32 i = 0; i < nbr; i++ )
        {
            if ((*properties)[i].getInterval() > 0)
            {
                m_Values[i] += (*properties)[i].getInterval();

                if (m_Values[i] > (*properties)[i].getMax())
                {
                    m_Values[i] = (*properties)[i].getMin();
                }
                else
                {
                    return true;
                }
            }
        }

        return false;
    }
}


template <class T>
T
Vector<T>::getDistanceEuclidean( const Vector & v ) const
{
    T d = (T) 0;
    T s;

    for( int32 i = 0; i < getDimension(); i++ )
    {
        s = m_Values[i]-v[i];
        d += s*s;
    }

    return sqrtf( d );
}


template <class T>
T
Vector<T>::getSquareDistanceEuclidean( const Vector & v ) const
{
    T d = (T) 0;
    T s;

    for( int32 i = 0; i < getDimension(); i++ )
    {
        s = (m_Values[i]-v[i]);
        d += s*s;
    }

    return d;
}


template <class T>
uint32
Vector<T>::load( FILE* fp )
{
    uint32 dim;

    fread( &dim, 1, sizeof( uint32 ), fp );
    setDimension( dim );

    for( int32 i = 0; i < m_Dimension; i++ )
    {
        fread( &m_Values[i], 1, sizeof( T ), fp );
    }

    return 1;
}


template <class T>
uint32
Vector<T>::save( FILE* fp ) const
{
    fwrite( &m_Dimension, 1, sizeof( uint32 ), fp );

    for( int32 i = 0; i < m_Dimension; i++ )
    {
        fwrite( &m_Values[i], 1, sizeof( T ), fp );
    }

    return 1;
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const Vector<T> & v )
{
    for( int32 i = 0; i < v.getDimension(); i++ )
    {
//        os.width( 15 );
        os << v[i] << " ";
    }

    return os;
}


template <class T>
QDebug
operator<<( QDebug os, const Vector<T> & v )
{
    for( int32 i = 0; i < v.getDimension(); i++ )
    {
//        os.nospace().width( 15 );
        os.nospace() << v[i] << " ";
    }

    return os.maybeSpace();
}
