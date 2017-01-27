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


#include "TemplateCS.h"


#include "Math/RandomNumberGenerator.h"


#include <ostream>
#include <QDebug>


const int32 SHIFT = 1;
const int32 NBR_DECISIONS = 12;


TemplateCS::TemplateCS()
{
    RandomNumberGenerator::initSeed( 0 );

    m_NextResponse.setDimension( 1 );
}

TemplateCS::~TemplateCS()
{
}


const char*
TemplateCS::getBaseName() const
{
    return "TemplateCS";
}


const char*
TemplateCS::getDescription() const
{
    return "";
}


const char*
TemplateCS::getAuthors() const
{
    return "Name1, Name2";
}


void
TemplateCS::defineParameters()
{
}


bool
TemplateCS::attachView( uint32 view_id, QWidget* widget )
{
    return true;
}


//*****************************************************************************


bool
TemplateCS::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager*  )
{
    return true;
}


void
TemplateCS::setMode( bool learning_mode )
{
}


bool
TemplateCS::canReset() const
{
    return true;
}


void
TemplateCS::reset()
{
}


void
TemplateCS::startNewSimulation()
{
}

void
TemplateCS::endSimulation()
{
}


void
TemplateCS::startNewEpisode( uint64 scenario_uid )
{
}

void
TemplateCS::endEpisode( bool is_success, uint32 nbr_decisions )
{
}


void
TemplateCS::observe( const Sensation & x )
{
    m_CurrentStimulus = x;

    cout << m_CurrentStimulus << endl;
}


const Response &
TemplateCS::selectResponse()
{
    m_NextResponse[0] = SHIFT+RandomNumberGenerator::getRandomNumber( (int32) NBR_DECISIONS );

    return m_NextResponse;
}


void
TemplateCS::suggestResponse( const Response & response )
{
}


void
TemplateCS::loadMemory()
{
}

void
TemplateCS::saveMemory()
{
}


void
TemplateCS::loadStats()
{
}

void
TemplateCS::saveStats()
{
}


//*****************************************************************************
