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


#ifndef __QUATERNION__
#define __QUATERNION__


#include "Types.h"
#include "Vectors.h"

#include <math.h>


typedef float32 quatType;

const quatType cstTolerance = 0.00001f;


#define quatAbs fabs
#define quatAcos acosf
#define quatCos cosf
#define quatSin sinf
#define quatSqrt sqrtf


class Quat
{
	public:
		Quat();
		Quat( quatType w, quatType x, quatType y, quatType z, bool fromAxis = false );

		Quat operator*( const Quat & ) const;
		Vec3 operator*( const Vec3 & v ) const;

		Quat operator+( const Quat & ) const;
		Quat operator*( float32 ) const;

		Quat & operator*=( const Quat & );

		float32 dot( const Quat & ) const;

		void set( quatType w, quatType x, quatType y, quatType z );
		void setFromAxis( quatType angle, quatType ax, quatType ay, quatType az );

		Quat conjuguate() const;

		void slerp( const Quat & a, const Quat & b, float32 t );
		void nlerp( const Quat & a, const Quat & b, float32 t );

		void normalize();

		void setRotationMatrix( quatType* mat ) const;

		void display();

	public:
		quatType w, x, y, z;
};


#endif
