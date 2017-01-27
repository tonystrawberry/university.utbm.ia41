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


#include "Response.h"


Response Response::VoidResponse;


Response::Response() :
    Ipseity::Vector()
{
}

Response::Response( int32 dim ) :
    Ipseity::Vector( dim )
{
}


std::ostream &
operator<<( std::ostream & os, const Response & a )
{
    os << (Ipseity::Vector) a;

    return os;
}


QDebug
operator<<( QDebug os, const Response & a )
{
    os.nospace() << static_cast<Ipseity::Vector>( a );

    return os.maybeSpace();
}
