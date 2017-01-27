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


template <class T>
void
ScalarDomain<T>::init( T min, T max, T interval )
{
    Min = min;
    Max = max;
    Interval = interval;

    computeCardinality();
}


template <class T>
void
ScalarDomain<T>::setPeriodic( bool periodic )
{
    Periodic = periodic;
}


template <class T>
const Cardinality &
ScalarDomain<T>::getCardinality() const
{
    return m_Cardinality;
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const ScalarDomain<T> & domain )
{
    os << "[" << domain.Min << ";" << domain.Max << "] ";
    os << "int=" << domain.Interval;

    return os;
}


template <class T>
QDebug
operator<<( QDebug os, const ScalarDomain<T> & domain )
{
    os.nospace() << "[" << domain.Min << ";" << domain.Max << "] ";
    os.nospace() << "int=" << domain.Interval;

    return os.maybeSpace();
}


//*****************************************************************************


template <class T>
void
ScalarDomain<T>::computeCardinality()
{
    if (Interval > ((T) 0))
    {
        m_Cardinality.Class = CARDINALITY_FINITE;
        m_Cardinality.Count = (uint64) (1+((Max-Min)/Interval));
    }
    else
    {
        m_Cardinality.Class = CARDINALITY_INFINITE;
        m_Cardinality.Count = 0;
    }
}
