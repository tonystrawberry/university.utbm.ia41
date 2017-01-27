/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2014, 2013 Ipseity Core Developers
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


#include "TreeEnsemble.h"


TreeEnsemble::TreeEnsemble( uint32 nbr_trees ) :
    m_NbrTrees( nbr_trees ),
    m_Trees( new ExtremelyRandomizedTree [nbr_trees] )
{
}


TreeEnsemble::~TreeEnsemble()
{
    delete [] m_Trees;
}


void
TreeEnsemble::create( uint32 nbr_cuts, uint32 min_samples, uint32 max_levels )
{
    for( uint32 i = 0; i < m_NbrTrees; ++i )
    {
        m_Trees[i].create( nbr_cuts, min_samples, max_levels );
    }
}


float64
TreeEnsemble::getLabel( float64* x )
{
    float64 frac_nbr_trees = 1.0 / ((float64) m_NbrTrees);
    float64 y;

    y = 0.0;
    for( uint32 i = 0; i < m_NbrTrees; ++i )
    {
        y += m_Trees[i].getLabel( x );
    }
    y *= frac_nbr_trees;

    return y;
}
