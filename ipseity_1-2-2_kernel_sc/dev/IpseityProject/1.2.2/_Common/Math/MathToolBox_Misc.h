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


#ifndef __MTB_MISC__
#define __MTB_MISC__


#include <math.h>

#include "Types.h"


#define PI				3.14159265359
#define PIOVER180		(PI/180)
#define NAPIER_NUMBER	2.7182818284


#define MAX( a, b )  (((a) > (b)) ? (a) : (b))
#define MIN( a, b )  (((a) < (b)) ? (a) : (b))
#define ABS( a ) ((((a) < 0) ? (-(a)) : (a)))
#define SIGN( a ) (((a) < 0) ? -1 : ((a) > 0) ? 1 : 0)

// Round Number Functions: keep a number proportional to f

float32 roundNumber( float32 n, float32 f );


// Combinatorics Functions

extern uint32 Factorial( uint32 );      // factorial function
extern uint32 Ank( uint32, uint32 );    // number of arrangements
extern uint32 Cnk( uint32, uint32 );    // number of combinations
extern uint32 CombinationPairID( uint32 n, uint32 n1, uint32 n2, bool oriented = false );

extern uint32 IntPower( uint32, uint32 );
extern float32 Float32Power( float32, uint32 );
extern float64 Float64Power( float64, uint32 );

#define Power2( n ) IntPower( 2, n )


// Arithmetic Functions

inline void PositiveSubstract( float32 & x, const float32 v );
inline int intervalComparison( float32 x, float32 lowerBound, float32 upperBound );
inline float32 scalarRandomWalk( const float32 initial, const float32 walkspeed, const float32 min, const float32 max );


template<class T>
inline void BoundedAdd( T & x, const T & v, const T & l , const T & u  )
{
	x += v;

	if (x < l)
	{
		x = l;
	}
	else if (x > u)
	{
		x = u;
	}
}

template<class T>
inline T clip( const T & x, const T & min, const T & max )
{
	if( x < min ) return min;
	if( x > max ) return max;
	return x;
}

template<class T>
inline T interpolate( float32 alpha, const T & x0, const T & x1 )
{
	return x0 + ((x1 - x0) * alpha);
}

template<class T>
inline void blendIntoAccumulator( const float32 smoothRate, const T & newValue, T & smoothedAccumulator )
{
	smoothedAccumulator = interpolate( clip( smoothRate, 0.0f, 1.0f ), smoothedAccumulator, newValue );
}

template<class T>
inline T range2unit( const T & p, const T & min, const T & range ) // [min;min+range] => [-1;1]
{
    T x;

    x = p-min;
    x /= range;
    x *= 2.0;
    x -= 1.0;

    return x;
}


// Distance measure Functions

#define square( n ) (n)*(n)

#define SQ2DDistance( x1, z1, x2, z2 ) (square( (x1)-(x2) ) + square( (z1)-(z2) ))
#define SQ3DDistance( x1, y1, z1, x2, y2, z2 ) (square( (x1)-(x2) ) + square( (y1)-(y2) ) + square( (z1)-(z2) ))
#define SR2DDistance( x1, z1, x2, z2 ) sqrtf( SQ2DDistance( x1, z1, x2, z2 ) )
#define SR3DDistance( x1, y1, z1, x2, y2, z2 ) sqrtf( SQ3DDistance( x1, y1, z1, x2, y2, z2 ) )

#define SQLengthIn2D( x, z ) (square( x ) + square( z ))
#define SQLengthIn3D( x, y, z ) (square( x ) + square( y ) + square( z ))
#define LengthIn2D( x, z ) sqrtf( square( x ) + square( z ) )
#define LengthIn3D( x, y, z ) sqrtf( square( x ) + square( y ) + square( z ) )


#endif
