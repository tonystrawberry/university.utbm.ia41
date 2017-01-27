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


#ifndef GenericVector_h
#define GenericVector_h


#include "Types.h"
#include "Math/VectorDomain.h"

#include <iostream>
#include <QtDebug>


template <class T>
class Vector;

template <class T> std::ostream & operator<<( std::ostream & os, const Vector<T> & v );
template <class T> QDebug operator<<( QDebug os, const Vector<T> & v );


//! Generic vector, parameterized by a class \a T.
/*!
    \author Fabrice LAURI
*/

template <class T>
class Vector
{
    public:
        Vector();
        Vector( const Vector & );
        virtual ~Vector();
        Vector & operator=( const Vector & );
        Vector & operator=( T* array );

        //! Initialize a vector to a given dimension.
        /*!
                \param dimension Dimension of the vector.
        */
        Vector( int32 dimension );

        //! Get the dimension of a vector.
        /*!
        */
        int32 getDimension() const;

        //! Set the dimension of a vector.
        /*!
                \param dimension Dimension of the vector.
        */
        void setDimension( int32 dimension, bool stored = true );

        //! Clear the vector.
        /*!
        */
        void clear();

        //! Init the vector from a string.
        /*!
        */
        void init( const char* );

        //! Get the value of a component of the vector.
        /*!
            \param i i-th component.
            \return Value of the i-th component.
        */
        T & operator[]( int32 i ) const;

        //! Equality operator.
        bool operator==( const Vector & ) const;

        //! Inequality operator.
        bool operator!=( const Vector & ) const;

        //! Lower operator.
        bool operator<( const Vector & ) const;

        //! Addition.
        Vector & operator+=( const Vector & );

        //! Component-wise multiplication and affectation.
        Vector & operator*=( const Vector & );

        //! Factor multiplication and affectation.
        Vector & operator*=( const T & );

        //! Component-wise multiplication.
        Vector operator*( const Vector & ) const;

        //! Component-wise division.
        Vector operator/( const Vector & ) const;

        //! Scalar multiplication.
        Vector operator*( const T & ) const;

        //! Concat a vector to the current one.
        Vector concat( const Vector & ) const;

        //! Set all the components of the vector to the same value.
        void fill( T );

        void setToMinVector( VectorDomain<T>* domain );
        void setToMaxVector( VectorDomain<T>* domain );
        void setRandomVector( VectorDomain<T>* domain );
        bool inc( VectorDomain<T>* domain );

        T getDistanceEuclidean( const Vector & ) const;
        T getSquareDistanceEuclidean( const Vector & ) const;

        uint32 load( FILE* );
        uint32 save( FILE* ) const;

        friend std::ostream & operator<< <>( std::ostream & os, const Vector<T> & v );
        friend QDebug operator<< <>( QDebug os, const Vector<T> & v );


    protected:
        int32 m_Dimension;
        bool m_Stored;
        T* m_Values;
};


#include "Vector.inl"


#endif
