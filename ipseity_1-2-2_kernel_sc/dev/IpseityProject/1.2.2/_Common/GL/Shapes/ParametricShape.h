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


#ifndef Shape_ParametricShape_h
#define Shape_ParametricShape_h


#include "Shape.h"

#include <vector>


class ParametricShape : public Shape
{
    public:
        ParametricShape( int nbr_vertices );
        ~ParametricShape();

    protected:
        void initArrays();

        void initVertex( int pos, GLfloat x, GLfloat y, GLfloat z );
        void generatePointsOnCircle( int pos, int shift, int nbr_edges, GLfloat radius, GLfloat angle_shift, GLfloat y, bool include_center );

        void generateColors( int first_index, int nbr_colors, GLfloat r, GLfloat g, GLfloat b );

        void createTabIndices( GLuint* & tab_indices, int size );

        void generateCircleIndices( GLuint* tab_indices, GLuint center_index, GLuint first_index, GLuint stride, int nbr_edges );
        void generateExtrusionIndices( GLuint* tab_indices, GLuint first_index, GLuint stride, int nbr_edges );
        void generateArmsIndices( GLuint* tab_indices, GLuint first_index, int nbr_arms );


    protected:
        GLfloat* m_TabVertices;
        GLfloat* m_TabColors;
        std::vector<GLuint*> m_TabIndices;
};


#endif
