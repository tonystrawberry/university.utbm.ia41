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


#include "RealTimeSearch.h"


#include "Math/RandomNumberGenerator.h"


#include <ostream>
#include <QDebug>


RealTimeSearch::RealTimeSearch()
{
    RandomNumberGenerator::initSeed( 0 );

    m_NextResponse.setDimension( 1 );
}

RealTimeSearch::~RealTimeSearch()
{
}


const char*
RealTimeSearch::getBaseName() const
{
    return "RealTimeSearch";
}


const char*
RealTimeSearch::getDescription() const
{
    return "";
}


const char*
RealTimeSearch::getAuthors() const
{
    return "Name1, Name2";
}


void
RealTimeSearch::defineParameters()
{
}


bool
RealTimeSearch::attachView( uint32 view_id, QWidget* widget )
{
    return true;
}


//*****************************************************************************


bool
RealTimeSearch::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager*  )
{
    return true;
}


bool
RealTimeSearch::canReset() const
{
    return true;
}


void
RealTimeSearch::reset()
{
}


void
RealTimeSearch::startNewSimulation()
{
}

void
RealTimeSearch::endSimulation()
{
}


void
RealTimeSearch::startNewEpisode( uint64 scenario_uid )
{
}

void
RealTimeSearch::endEpisode( bool is_success, uint32 nbr_decisions )
{
}


void
RealTimeSearch::observe( const Sensation & x )
{
    m_CurrentStimulus = x;

    cout << m_CurrentStimulus << endl;
}


const Response &
RealTimeSearch::selectResponse()
{
    m_NextResponse[0] = RandomNumberGenerator::getRandomNumber( 5 );

    return m_NextResponse;
}


void
RealTimeSearch::suggestResponse( const Response & response )
{
}


void
RealTimeSearch::loadMemory()
{
}

void
RealTimeSearch::saveMemory()
{
}


void
RealTimeSearch::loadStats()
{
}

void
RealTimeSearch::saveStats()
{
}


//*****************************************************************************
