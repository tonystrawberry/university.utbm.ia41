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

 
#include "BoundedVariable.h"


template <class T>
BoundedVariable<T>::BoundedVariable()
{
    m_Value = 0.0;
    m_Min = -1.0;
    m_Max = 1.0;
}


template <class T>
BoundedVariable<T>::BoundedVariable( T min, T max )
{
    m_Value = m_Min = min;
    m_Max = max;
}


template <class T>
T &
BoundedVariable<T>::operator()()
{
    return m_Value;
}


template <class T>
BoundedVariable<T> &
BoundedVariable<T>::operator=( T new_value )
{
    m_Value = new_value;

    if (m_Value < m_Min)
    {
        m_Value = m_Min;
    }
    else if (m_Value > m_Max)
    {
        m_Value = m_Max;
    }

    return (*this);
}


template <class T>
BoundedVariable<T> &
BoundedVariable<T>::operator+=( T value )
{
    m_Value += value;

    if (m_Value < m_Min)
    {
        m_Value = m_Min;
    }
    else if (m_Value > m_Max)
    {
        m_Value = m_Max;
    }

    return (*this);
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const BoundedVariable<T> & pv )
{
    os << pv.m_Value;

    return os;
}
