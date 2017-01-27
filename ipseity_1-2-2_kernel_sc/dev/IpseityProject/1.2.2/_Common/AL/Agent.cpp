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


#include "Agent.h"

#include "Types.h"
#include "AL/AbstractEnvironment.h"
#include "IO/VFiles.h"
#include "IO/GnuplotInterface.h"

#include <fstream>


Agent::Agent( uint32 taxon_id ) :
    m_TaxonId( taxon_id ),
    m_State( 0 )
{
}


const char*
Agent::getName() const
{
    return m_Name.c_str();
}


void
Agent::setName( const char* name )
{
    m_Name = name;
}


uint32
Agent::getTaxonId() const
{
    return m_TaxonId;
}


bool
Agent::isTerminal() const
{
    return Terminal;
}


bool
Agent::isGoalReached() const
{
    return GoalReached;
}


void
Agent::setInteractionState( uint32 state )
{
    m_State = state;
}
