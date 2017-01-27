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

 
#ifndef Variable_h
#define Variable_h


#include "AL/IpseityTypes.h"


namespace Ipseity
{
    typedef enum {
        VariableScalar  = 0,
        VariableString,
        VariableVector,
        VariableMatrix
    } VariableType;

    typedef enum {
        VariableIn   = 0,
        VariableOut,
        VariableParameter
    } VariableStatus;
}


#define GET_FUNCTION_DEF( class_name ) \
    bool fetchValue( class_name & value ); \

#define SET_FUNCTION_DEF( class_name ) \
    void setValue( const class_name & value );


class Variable
{
    public:
        Variable( const char* name, Ipseity::VariableType type, Ipseity::VariableStatus status );
        ~Variable();

        const char* getName() const;

        GET_FUNCTION_DEF( Ipseity::Scalar )
        SET_FUNCTION_DEF( Ipseity::Scalar )

        GET_FUNCTION_DEF( Ipseity::String )
        SET_FUNCTION_DEF( Ipseity::String )

        GET_FUNCTION_DEF( Ipseity::Vector )
        SET_FUNCTION_DEF( Ipseity::Vector )

        const Ipseity::Scalar & getScalar() const;
        const Ipseity::String & getString() const;
        const Ipseity::Vector & getVector() const;

    protected:
        void init();


    protected:
        Ipseity::String m_Name;
        Ipseity::VariableType m_Type;
        void* m_Value;
        Ipseity::VariableStatus m_Status;
};


#endif
