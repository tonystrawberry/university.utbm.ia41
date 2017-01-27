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


#ifndef VectorDomain_h
#define VectorDomain_h


#include "Types.h"
#include "IpseityString.h"
#include "MemDefns.h"
#include "ScalarDomain.h"
#include "Cardinality.h"

#include <iostream>
#include <QDebug>


using namespace std;


template <class T>
class Vector;

template <class T>
class VectorDomain;

template <class T> std::ostream & operator<<( std::ostream & os, const VectorDomain<T> & v );
template <class T> QDebug operator<<( QDebug os, const VectorDomain<T> & v );


//! Vector domain, parameterized by a class \a T.
/*!
    This class contains the value interval of each component of a scalar vector.
    \author Fabrice LAURI
*/

template <class T>
class VectorDomain
{
    public:
        VectorDomain();
        VectorDomain( const VectorDomain & );
        virtual ~VectorDomain();
        VectorDomain & operator=( const VectorDomain & );

        //! Build a vector of a given dimension.
        /*!
            \param dimension Dimension of the vector.
        */
        VectorDomain( int32 dimension );

        //! Get the dimension.
        /*!
        */
        int32 getDimension() const;

        //! Set the dimension of the vector.
        /*!
            \param dimension Dimension of the vector.
        */
        void setDimension( int32 dimension );

        //! Set the domain of a given vector component.
        void setDomain( int32 variable_id, const ScalarDomain<T> & domain );

        //! Get the domain of a vector component.
        /*!
            \param i i-th domain.
            \return Domain of the i-th component.
        */
        const ScalarDomain<T> & operator[]( int32 i ) const;

        //! Concat a vector with the current one.
        VectorDomain concat( const VectorDomain & ) const;

        //! Get the cardinality of the space spanned by the vector.
        Cardinality getCardinality() const;

        uint32 getIndex( const Vector<T> & v ) const;

        bool query( const char* name, uint32 & id ) const;

        uint32 load( FILE* );
        uint32 save( FILE* ) const;

        friend std::ostream & operator<< <>( std::ostream & os, const VectorDomain<T> & v );
        friend QDebug operator<< <>( QDebug os, const VectorDomain<T> & v );

    protected:
        //! Clear the vector.
        void clear();


    public:
        Ipseity::String Name;

    protected:
        int32 m_Dimension;
        ScalarDomain<T>* m_Domains;
        Cardinality m_Cardinality;
};


#include "VectorDomain.inl"


#endif
