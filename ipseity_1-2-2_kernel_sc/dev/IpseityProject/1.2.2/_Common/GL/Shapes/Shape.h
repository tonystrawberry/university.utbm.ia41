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


#ifndef Shape_h
#define Shape_h


#include <GL/OpenGL.h>


#include "Types.h"
#include "AbstractShape.h"


class Shape : public AbstractShape
{
    public:
		Shape();
		virtual ~Shape();

		void clear();

		uint32 getNbrVertices() const;
		const GLfloat* vertices() const;
		const GLfloat* normals() const;
		const GLfloat* textureCoordinates() const;

		uint32 getNbrIndices() const;
		const GLuint* indices() const;


    protected:
		bool m_DynamicVertices;
		uint32 m_NbrVertices;
		GLfloat* m_TabVertices;
		GLfloat* m_TabNormals;
		GLfloat* m_TabTexCoords;
		bool m_DynamicIndices;
		uint32 m_NbrIndices;
		GLuint* m_TabIndices;
};


#endif
