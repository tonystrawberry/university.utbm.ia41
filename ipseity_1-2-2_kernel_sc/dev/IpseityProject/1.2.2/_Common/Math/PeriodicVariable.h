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


#ifndef PeriodicVariable_h
#define PeriodicVariable_h


#include <ostream>


template <class T>
class PeriodicVariable;

template <class T> std::ostream & operator<<( std::ostream & os, const PeriodicVariable<T> & v );


template <class T>
class PeriodicVariable
{
    public:
        PeriodicVariable();
        PeriodicVariable( T min, T max );

        PeriodicVariable & operator=( T v );
        PeriodicVariable & operator+=( T v );

        T & operator()();

        friend std::ostream & operator<< <>( std::ostream &, const PeriodicVariable<T> & );


    protected:
        T m_Value;
        T m_Min, m_Range;
};


#include "PeriodicVariable.inl"


#endif
