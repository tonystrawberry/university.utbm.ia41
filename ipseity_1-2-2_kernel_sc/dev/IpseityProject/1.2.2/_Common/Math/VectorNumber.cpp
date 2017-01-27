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

#include "VectorNumber.h"


#include <algorithm>
#include <iostream>

using namespace std;


VectorNumber::VectorNumber( int32 nbr_dimensions )
{
    m_Cardinalities.resize( nbr_dimensions, 1 );
    m_Number.resize( nbr_dimensions, 0 );
}


void
VectorNumber::setCardinality( int32 dimension_id, int32 cardinality )
{
    m_Cardinalities[dimension_id] = cardinality;
}


int32
VectorNumber::getCardinality() const
{
    int32 c = 1;

    for( uint32 i = 0; i < m_Cardinalities.size(); ++i )
    {
        c *= m_Cardinalities[i];
    }

    return c;
}


const std::vector<int32> &
VectorNumber::getNumber() const
{
    return m_Number;
}


void
VectorNumber::selectNumber( int32 number_id )
{
    std::fill( m_Number.begin(), m_Number.end(), 0 );

    if (number_id < getCardinality())
    {
        for( int32 i = 0; i < number_id && nextNumber(); ++i );
    }
}


bool
VectorNumber::nextNumber()
{
    uint32 i = 0;

    while( i < m_Cardinalities.size() )
    {
        if (m_Cardinalities[i] > 1)
        {
            ++m_Number[i];

            if (m_Number[i] < m_Cardinalities[i])
            {
                return true;
            }

            m_Number[i] = 0;
        }
        ++i;
    }

    return false;
}


void
VectorNumber::display()
{
    for( uint32 i = 0; i < m_Number.size(); ++i )
    {
        cout << m_Number[i] << " ";
    }
    cout << endl;
}
