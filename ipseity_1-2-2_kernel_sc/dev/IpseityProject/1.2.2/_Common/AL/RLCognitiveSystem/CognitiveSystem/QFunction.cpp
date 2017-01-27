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


#include "QFunction.h"

#include "MemDefns.h"
#include "StringFunctions.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include "IO/GnuplotInterface.h"
#include "XMLTags_CognitiveSystem.h"

#include <fstream>
#include <QDebug>


QFunction::QFunction( const char* memory_name, const char* base_path, const char* name ) :
    m_MemoryName( memory_name ),
    m_Path( base_path ),
    m_ConfigFileName( name ),
    m_Memory( NULL )
{
}

QFunction::~QFunction()
{
    m_QFunction.free();
}


const char*
QFunction::getBaseName() const
{
    return "QFunction";

    if (m_Memory)
    {
        return m_Memory->getBaseName();
    }
    else
    {
        return "Void";
    }
}


const char*
QFunction::getDescription() const
{
    return "";
}


const char*
QFunction::getAuthors() const
{
    return "Lauri, F. (fabrice.lauri@utbm.fr)";
}


void
QFunction::defineParameters()
{
}


bool
QFunction::attachView( uint32 view_id, QWidget* widget )
{
    qDebug() << "Attach:" << m_Memory->getName();
    m_Memory->attachView( 0, widget );

    return true;
}


bool
QFunction::canMemorize() const
{
    return (m_Memory != NULL);
}


void
QFunction::init( SensorySignalProperties* sensory_properties, ResponseSignalProperties* response_properties )
{
    m_SensoryProperties = sensory_properties;
    m_ResponseProperties = response_properties;

    m_QFunction.setParent( this );
    if (m_QFunction.load( "QValueMemory", m_MemoryName.c_str() ))
    {
        m_Memory = reinterpret_cast<AbstractQFunctionModule*>(m_QFunction.get());
        qDebug() << m_QFunction.getGroupName() << "'" << getName() << "' loaded!";

//		cout << "XP: " << (*x.getProperties()) << ", UP: " << (*u.getProperties()) << endl;

        m_Memory->setDataDescription( sensory_properties, response_properties );
        m_Memory->loadParameters( m_ConfigFileName.c_str(), m_Path.c_str() );

        if (m_Memory->isDynamic())
        {
            qDebug() << "# memory cells: Variable";
        }
        else
        {
            qDebug() << "# memory cells:" << getNbrCells();
        }
    }
}


SensorySignalProperties*
QFunction::getSensorySignalProperties() const
{
    return m_SensoryProperties;
}


ResponseSignalProperties*
QFunction::getResponseSignalProperties() const
{
    return m_ResponseProperties;
}


bool
QFunction::canReset() const
{
    return (canMemorize() && m_Memory->canReset());
}


void
QFunction::reset()
{
    if (m_Memory)
    {
       m_Memory->reset();
    }
}


uint32
QFunction::getNbrCells() const
{
    if (m_Memory)
    {
        return m_Memory->getNbrCells();
    }
    else
    {
        return 0;
    }
}


Ipseity::Scalar
QFunction::getValue( const SensorySignal & x, const Response & u )
{
    return (m_Memory ? m_Memory->getValue( x, u ) : 0.0);

//	cout << "Q(" << x << "," << u << "): " << q << endl;
}


void
QFunction::update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate )
{
    if (m_Memory)
    {
        m_Memory->update( x, u, old_estimate, new_estimate );
    }
}


void
QFunction::clearEligibilityTraces()
{
    if (m_Memory)
    {
        m_Memory->clearEligibilityTraces();
    }
}


void
QFunction::decayEligibilityTraces( Ipseity::Scalar factor )
{
    if (m_Memory)
    {
        m_Memory->decayEligibilityTraces( factor );
    }
}


void
QFunction::replaceEligibilityTraces( const SensorySignal & x, const Response & u )
{
    if (m_Memory)
    {
        m_Memory->replaceEligibilityTraces( x, u );
    }
}


void
QFunction::accumulateEligibilityTraces( const SensorySignal & x, const Response & u )
{
    if (m_Memory)
    {
        m_Memory->accumulateEligibilityTraces( x, u );
    }
}


//*****************************************************************************


bool
QFunction::load( const char* file_name )
{
    if (m_Memory)
    {
        return m_Memory->load( file_name );
    }
    else
    {
        return false;
    }
}


bool
QFunction::save( const char* file_name )
{
    if (m_Memory)
    {
        return m_Memory->save( file_name );
    }
    else
    {
        return false;
    }
}


//*****************************************************************************
