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


#ifndef GenericMatrix_h
#define GenericMatrix_h


#include "GenericVector.h"

#include <ostream>


typedef enum {
    IM_LU = 0,
    IM_SVD
} TInversionMethod;


template <class T>
class GenericMatrix
{
    public:
        static void setInversionMethod( uint32 );
        static void setDiagonalValue( T );

        GenericMatrix();
        GenericMatrix( int32, int32 );
        GenericMatrix( const GenericMatrix & );
        virtual ~GenericMatrix();

        int32 getNbrRows() const;
        int32 getNbrColumns() const;

        T* & operator[]( int32 ) const;

        void setSize( int32, int32 );

        void fill( T );
        void setIdentity( T = 1 );

        GenericMatrix & operator=( const GenericMatrix & );
        GenericMatrix & operator=( const Vector<T> & );
        GenericMatrix & operator=( T );
        GenericMatrix & operator+=( const GenericMatrix & );
        GenericMatrix & operator*=( T );
        GenericMatrix operator+( const GenericMatrix & );
        GenericMatrix operator-( const GenericMatrix & );
        GenericMatrix operator*( const GenericMatrix & );
        GenericMatrix operator*( T );

        T maxValue() const;

        T dot( const GenericMatrix & ) const;

        GenericMatrix transpose() const;

        void initForLaterInversion();

        GenericMatrix invert() const;

        template <class T> friend std::ostream & operator<<( std::ostream & os, const GenericMatrix<T> & v );

    protected:
        void destroy();

    private:
        int svd( int m, int n, int withu, int withv, T eps, T tol, T **a, T *q, T **u, T **v) const;

        void ludcmp( T** a, int32 n, int32* indx, T* d ) const;
        void lubksb( T** a, int32 n, int32* indx, T* b ) const;

    protected:
        static uint32 m_InversionMethod;
        static T m_DiagonalValue;

        int32 m_NbrRows;
        int32 m_NbrColumns;
        T** m_Values;
};


#include "GenericMatrix.inl"


#endif
