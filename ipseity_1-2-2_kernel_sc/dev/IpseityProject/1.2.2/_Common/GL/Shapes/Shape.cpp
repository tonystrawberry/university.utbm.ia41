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


//*****************************************************************************
//
// Class: Shape
// Author: Fabrice LAURI (fabrice.lauri@utbm.fr)
//         (Associate Professor at the
//          "Université de Technologie de Belfort-Montbéliard", France)
// Version: 1.0
// Last update: 2012-08-09
//
//*****************************************************************************


#include "Shape.h"
#include "MemDefns.h"


Shape::Shape() :
    m_NbrVertices( 0 ),
    m_TabVertices( NULL ),
    m_TabNormals( NULL ),
    m_TabTexCoords( NULL ),
    m_NbrIndices( 0 ),
    m_TabIndices( NULL ),
    m_DynamicVertices( true ),
    m_DynamicIndices( true )
{
}

Shape::~Shape()
{
    clear();
}


uint32
Shape::getNbrVertices() const
{
    return m_NbrVertices;
}

const GLfloat*
Shape::vertices() const
{
    return m_TabVertices;
}

const GLfloat*
Shape::normals() const
{
    return m_TabNormals;
}

const GLfloat*
Shape::textureCoordinates() const
{
    return m_TabTexCoords;
}


uint32
Shape::getNbrIndices() const
{
    return m_NbrIndices;
}

const GLuint*
Shape::indices() const
{
    return m_TabIndices;
}


void
Shape::clear()
{
    if (m_DynamicVertices)
    {
        SafeDeleteArray( m_TabVertices );
        SafeDeleteArray( m_TabNormals );
        SafeDeleteArray( m_TabTexCoords );
    }

    if (m_DynamicIndices)
    {
        SafeDeleteArray( m_TabIndices );
    }
}
