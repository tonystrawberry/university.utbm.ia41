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


#include "VFunction.h"

#include "MemDefns.h"
#include "StringFunctions.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include "IO/GnuplotInterface.h"
#include "XMLTags_CognitiveSystem.h"

#include <fstream>


VFunction::VFunction( AbstractQFunction* qvm, AbstractResponseModule* rm )
{
    m_QValueMemory = qvm;
    m_ResponseModule = rm;
}

VFunction::~VFunction()
{
}


const char*
VFunction::getName() const
{
    return "Greedy V-Function";
}


void
VFunction::reset()
{
}


Ipseity::Scalar
VFunction::getValue( const SensorySignal & x )     // solve V(x) = max_{u \in U} Q(x,u) when U is finite
{
    Ipseity::Scalar q;
    Ipseity::Scalar qmax;

    m_Response = m_ResponseModule->selectFirstResponse( x );
    qmax = m_QValueMemory->getValue( x, m_Response );
    while( m_ResponseModule->selectNextResponse( x, m_Response ) )
    {
        q = m_QValueMemory->getValue( x, m_Response );

        if (q > qmax)
        {
            qmax = q;
        }
    }

    return qmax;
}


//*****************************************************************************


bool
VFunction::load( const char* file_name )
{
    return false;
}


bool
VFunction::save( const char* file_name )
{
    return false;
}
