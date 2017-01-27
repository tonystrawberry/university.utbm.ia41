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


#ifndef ScalarDomain_h
#define ScalarDomain_h


#include "IpseityString.h"
#include "Cardinality.h"

#include <QDebug>


template <class T>
class ScalarDomain;

template <class T> std::ostream & operator<<( std::ostream & os, const ScalarDomain<T> & v );
template <class T> QDebug operator<<( QDebug os, const ScalarDomain<T> & v );


//! Scalar domain, parameterized by a class \a T.
/*!
    This class represents a closed set (like [a;b] or {a_1, a_2, ..., a_n} with a_{i+1}-a_i = s, for all i, with s > 0).
    \author Fabrice LAURI
*/


template <class T>
class ScalarDomain
{
    public:
        //! Set the minimum value, the maximum value and the interval between two consecutive values.
        void init( T min, T max, T interval );

        //! Indicates whether the set is periodic.
        void setPeriodic( bool );

        T getMin() const        { return Min; }
        T getMax() const        { return Max; }
        T getInterval() const   { return Interval; }

        //! Get the cardinality of the set.
        const Cardinality & getCardinality() const;

        friend std::ostream & operator<< <>( std::ostream & os, const ScalarDomain<T> & v );
        friend QDebug operator<< <>( QDebug os, const ScalarDomain<T> & v );


    protected:
        void computeCardinality();

    public:
        Ipseity::String Name;

    protected:
        T Min;
        T Max;
        T Interval;
        bool Periodic;
        Cardinality m_Cardinality;
};


#include "ScalarDomain.inl"


#endif
