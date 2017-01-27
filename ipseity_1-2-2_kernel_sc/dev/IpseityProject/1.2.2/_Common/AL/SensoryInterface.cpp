/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2015, 2014, 2013 Ipseity Core Developers
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

 
#include "SensoryInterface.h"


#include <iostream>

using namespace std;


bool
SensoryInterface::operator==( const SensoryInterface & si ) const
{
    if (SensorySignals.size() == si.SensorySignals.size())
    {
        std::vector<Ipseity::String>::const_iterator itr1;
        std::vector<Ipseity::String>::const_iterator itr2;

        for( itr1 = SensorySignals.begin(), itr2 = si.SensorySignals.begin();
             itr1 != SensorySignals.end(), itr2 != si.SensorySignals.end(); ++itr1, ++itr2 )
        {
            if ((*itr1) != (*itr2))
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool
SensoryInterface::operator!=( const SensoryInterface & si ) const
{
    return !SensoryInterface::operator==( si );
}


void
SensoryInterface::add( const char* sensory_signal_name )
{
    SensorySignals.push_back( sensory_signal_name );
}


void
SensoryInterface::display() const
{
    std::vector<Ipseity::String>::const_iterator itr;
    uint32 k;

    for( k = 0, itr = SensorySignals.begin(); itr != SensorySignals.end(); ++k, ++itr )
    {
        cout << k << ": " << (*itr) << endl;
    }
}


bool
SensoryInterface::checkCompatibility( const SensoryInterface & sensory_interface ) const
{
    uint32 member_nbr_signals;

    member_nbr_signals = SensorySignals.size();

    if (sensory_interface.SensorySignals.size() <= member_nbr_signals)
    {
        uint32 j, k;
        uint32 nbr_signals;

        for ( k = 0; k < sensory_interface.SensorySignals.size(); ++k )
        {
            j = 0;
            nbr_signals = sensory_interface.SensorySignals.size();

            while (j < nbr_signals && SensorySignals[j] != sensory_interface.SensorySignals[k])
            {
                ++j;
            }

            if (j == member_nbr_signals)
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}
