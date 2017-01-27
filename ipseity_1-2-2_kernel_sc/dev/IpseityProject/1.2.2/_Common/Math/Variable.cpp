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

 
#include "Variable.h"


#define GET_FUNCTION( class_name, type ) \
    bool \
    Variable::fetchValue( class_name & value ) \
    { \
        if (m_Type == type) \
        { \
            value = (*reinterpret_cast<class_name*> (m_Value)); \
            return true; \
        } \
        else \
        { \
            return false; \
        } \
    } \

#define SET_FUNCTION( class_name, type ) \
    void \
    Variable::setValue( const class_name & value ) \
    { \
        if (m_Type == type) \
        { \
            (*reinterpret_cast<class_name*> (m_Value)) = value; \
        } \
    } \


Variable::Variable( const char* name, Ipseity::VariableType type, Ipseity::VariableStatus status ) :
    m_Name( name ),
    m_Type( type ),
    m_Status( status )
{
    init();
}

Variable::~Variable()
{
    delete m_Value;
}


const char*
Variable::getName() const
{
    return m_Name.c_str();
}


GET_FUNCTION( Ipseity::Scalar, Ipseity::VariableScalar )
GET_FUNCTION( Ipseity::String, Ipseity::VariableString )
GET_FUNCTION( Ipseity::Vector, Ipseity::VariableVector )

SET_FUNCTION( Ipseity::Scalar, Ipseity::VariableScalar )
SET_FUNCTION( Ipseity::String, Ipseity::VariableString )
SET_FUNCTION( Ipseity::Vector, Ipseity::VariableVector )


const Ipseity::Scalar &
Variable::getScalar() const
{
    return  (*reinterpret_cast<Ipseity::Scalar*> (m_Value));
}

const Ipseity::String &
Variable::getString() const
{
    return  (*reinterpret_cast<Ipseity::String*> (m_Value));
}

const Ipseity::Vector &
Variable::getVector() const
{
    return  (*reinterpret_cast<Ipseity::Vector*> (m_Value));
}


//*****************************************************************************

void
Variable::init()
{
    switch( m_Type )
    {
        case Ipseity::VariableScalar:
            m_Value = new Ipseity::Scalar;
            break;

        case Ipseity::VariableString:
            m_Value = new Ipseity::String();
            break;

        case Ipseity::VariableVector:
            m_Value = new Ipseity::Vector;
            break;

        case Ipseity::VariableMatrix:
//            m_Value = new Ipseity::Vector;
            break;
    }
}
