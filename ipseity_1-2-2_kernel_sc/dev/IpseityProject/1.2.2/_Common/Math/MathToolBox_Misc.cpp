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


#include "MathToolBox_Misc.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Types.h"
#include "MemDefns.h"
#include "RandomNumberGenerator.h"


// Round Number Functions: keep a number proportional to f

float32
roundNumber( float32 n, float32 f )
{
    float32 ip, fp;

    ip = floor( n );
    fp = n - ip;
    fp = floor( fp/f )*f;

    return ip+fp;
}


// Combinatorics Functions

uint32
Factorial( uint32 n )
{
    if (n < 2)
    {
        return 1;
    }
    else
    {
        uint32 v = 2;

        for( uint32 i = 3; i <= n; ++i )
        {
            v *= i;
        }

        return v;
    }
}


uint32
Ank( uint32 n, uint32 k )
{
    return (Factorial( n ) / Factorial( n-k ));
}


uint32
Cnk( uint32 n, uint32 k )
{
    return (Factorial( n ) / (Factorial( k )*Factorial( n-k )));
}


uint32
CombinationPairID( uint32 n, uint32 n1, uint32 n2, bool oriented ) // we suppose n1 == n2 is impossible
{
    uint32 nr = 0;

    if (n1 > n2)
    {
        uint32 i = n1;
        n1 = n2;
        n2 = i;

        if (oriented)
        {
            nr = (n*(n-1))/2;
        }
    }

    return nr + (n*n1) - ((n1*(n1+1))/2) + (n2-n1-1);
}


uint32
IntPower( uint32 v, uint32 n )
{
    if( n == 0 )
    {
        return 1;
    }
    else if( n == 1 )
    {
        return v;
    }
    else
    {
        uint32 r;

        r = v;
        for( uint32 nr = 1; nr < n; nr++ )
        {
            r *= v;
        }

        return r;
    }
}


float32
Float32Power( float32 v, uint32 n )
{
    if( n == 0 )
    {
        return 1.0f;
    }
    else if( n == 1 )
    {
        return v;
    }
    else
    {
        float32 r = v;

        for( uint32 nr = 1; nr < n; nr++ )
        {
            r *= v;
        }

        return r;
    }
}


float64
Float64Power( float64 v, uint32 n )
{
    if( n == 0 )
    {
        return 1.0;
    }
    else if( n == 1 )
    {
        return v;
    }
    else
    {
        float64 r = v;

        for( uint32 nr = 1; nr < n; nr++ )
        {
            r *= v;
        }

        return r;
    }
}



/*
float GetPositiveAngle( float a )
{
    return (a < 0) ? (2*PI+a) : a;
}
*/


void
PositiveSubstract( float32 & x, const float32 v )
{
    x -= v;
    if (x < 0.0f)
    {
        x = 0.0f;
    }
}

int
intervalComparison( float32 x, float32 lowerBound, float32 upperBound )
{
    if( x < lowerBound ) return -1;
    if( x > upperBound ) return +1;
    return 0;
}


float32
scalarRandomWalk( const float32 initial, const float32 walkspeed, const float32 min, const float32 max )
{
    const float32 next = initial + (RandomNumberGenerator::getRandomNumber( -1.0f, 1.0f ) * walkspeed);

    return clip( next, min, max );
}
