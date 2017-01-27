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


#include "PeriodicVariable.h"


#include "Types.h"
#include "MathToolBox_Misc.h"


template <class T>
PeriodicVariable<T>::PeriodicVariable()
{
    m_Value = m_Min = 0.0;
    m_Range = 2.0*PI;
}


template <class T>
PeriodicVariable<T>::PeriodicVariable( T min, T max )
{
    m_Value = m_Min = min;
    m_Range = max-min;
}


template <class T>
T &
PeriodicVariable<T>::operator()()
{
    return m_Value;
}


template <class T>
PeriodicVariable<T> &
PeriodicVariable<T>::operator=( T new_value )
{
    T x;

    x = new_value-m_Min;
    m_Value = x-m_Range*floor(x/m_Range);
    m_Value += m_Min;

    return (*this);
}


template <class T>
PeriodicVariable<T> &
PeriodicVariable<T>::operator+=( T value )
{
    T x;

    x = (m_Value+value)-m_Min;
    m_Value = x-m_Range*floor(x/m_Range);
    m_Value += m_Min;

    return (*this);
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const PeriodicVariable<T> & pv )
{
    os << pv.m_Value;

    return os;
}
