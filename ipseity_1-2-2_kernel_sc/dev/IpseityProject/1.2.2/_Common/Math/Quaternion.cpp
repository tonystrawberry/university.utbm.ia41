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


#include "Quaternion.h"


Quat::Quat()
{
    w = 1.0f;
    x = y = z = 0.0f;
}

Quat::Quat( quatType w, quatType x, quatType y, quatType z, bool bfa )
{
    if (bfa)
    {
        setFromAxis( w, x, y, z );
    }
    else
    {
        set( w, x, y, z );
    }
}


Quat
Quat::operator*( float32 v ) const
{
    return Quat( w*v, x*v, y*v, z*v );
}


Quat
Quat::operator+( const Quat & a ) const
{
    return Quat( w+a.w, x+a.x, y+a.y, z+a.z );
}


Quat
Quat::operator*( const Quat & q ) const
{
    Quat qr( w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w );

    qr.normalize();

    return qr;
}

Vec3
Quat::operator*( const Vec3 & v ) const
{
    Quat vecQuat( 0.0f, v.x, v.y, v.z ), resQuat;

    vecQuat.normalize();

    resQuat = vecQuat * conjuguate();
    resQuat = *this * resQuat;

    return Vec3( resQuat.x, resQuat.y, resQuat.z );
}

Quat &
Quat::operator*=( const Quat & q )
{
    quatType w1, x1, y1, z1;

    w1 = w*q.w - x*q.x - y*q.y - z*q.z;
    x1 = w*q.x + x*q.w + y*q.z - z*q.y;
    y1 = w*q.y - x*q.z + y*q.w + z*q.x;
    z1 = w*q.z + x*q.y - y*q.x + z*q.w;

    w = w1;
    x = x1;
    y = y1;
    z = z1;

    normalize();

    return *this;
}


float32
Quat::dot( const Quat & a ) const
{
    return (w*a.w+x*a.x+y*a.y+z*a.z);
}


void
Quat::set( quatType w, quatType x, quatType y, quatType z )
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}


void
Quat::setFromAxis( quatType theta, quatType ax, quatType ay, quatType az )
{
    quatType ha = theta/2;
    quatType a = quatSin( ha );

    w = quatCos( ha );
    x = a*ax;
    y = a*ay;
    z = a*az;
}


Quat
Quat::conjuguate() const
{
    return Quat( w, -x, -y, -z );
}


void
Quat::normalize()
{
    quatType l = w*w + x*x +y*y + z*z;

    if( quatAbs( l - 1.0f ) > cstTolerance )
    {
        l = (1.0f / quatSqrt( l ));

        w *= l;
        x *= l;
        y *= l;
        z *= l;
    }
}


void
Quat::setRotationMatrix( quatType* mat ) const
{
    quatType xx, xy, xz, xw, yy, yz, yw, zz, zw;

    xx      = x * x;
    xy      = x * y;
    xz      = x * z;
    xw      = x * w;

    yy      = y * y;
    yz      = y * z;
    yw      = y * w;

    zz      = z * z;
    zw      = z * w;

    mat[0]  = 1 - 2 * ( yy + zz );
    mat[4]  =     2 * ( xy - zw );
    mat[8]  =     2 * ( xz + yw );

    mat[1]  =     2 * ( xy + zw );
    mat[5]  = 1 - 2 * ( xx + zz );
    mat[9]  =     2 * ( yz - xw );

    mat[2]  =     2 * ( xz - yw );
    mat[6]  =     2 * ( yz + xw );
    mat[10] = 1 - 2 * ( xx + yy );

    mat[3] = mat[7] = mat[11] = 0.0f;
    mat[12]  = mat[13] = mat[14] = 0.0f;
    mat[15] = 1.0f;
}


void
Quat::display()
{
    printf( "%.2f,%.2f,%.2f,%.2f\n", w, x, y, z );
}


// ------------------------------------
// Creates a value for this Quat from spherical linear interpolation
// t is the interpolation value from 0 to 1
// ------------------------------------
void
Quat::slerp( const Quat & a, const Quat & b, float t )
{
    quatType w1, w2;

    quatType cosTheta = a.dot( b );
    quatType theta    = quatAcos( cosTheta );
    quatType sinTheta = quatSin( theta );

    if( sinTheta > 0.001f )
    {
        w1 = quatSin( (1.0f-t)*theta ) / sinTheta;
        w2 = quatSin( t*theta) / sinTheta;
    }
    else
    {
        // CQuat a ~= CQuat b
        w1 = 1.0f - t;
        w2 = t;
    }

    *this = a*w1 + b*w2;

    normalize();
}

// ------------------------------------
// linearly interpolate each component, then normalize the Quat
// Unlike spherical interpolation, this does not rotate at a constant velocity,
// although that's not necessarily a bad thing
// ------------------------------------
void
Quat::nlerp( const Quat & a, const Quat & b, float32 t )
{
    *this = a*(1.0f-t) + b*t;
    normalize();
}

/*
Quat Quat::slerp( const Quat & v0, const Quat & v1, quatType t )
{
    // v0 and v1 should be unit length or else
    // something broken will happen.

    // Compute the cosine of the angle between the two vectors.
    quatType dot = dot_product( v0, v1 );

    const quatType DOT_THRESHOLD = 0.9995f;
    if( dot > DOT_THRESHOLD )
    {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        Quat result = v0 + t*(v1 – v0);
        result.normalize();

        return result;
    }

    Clamp( dot, -1, 1 );         	// Robustness: Stay within domain of acos()
    double theta_0 = acos( dot );	// theta_0 = angle between input vectors
    double theta = theta_0*t;    	// theta = angle between v0 and result

    Quat v2 = v1 – v0*dot;
    v2.normalize();              // { v0, v2 } is now an orthonormal basis

    return v0*cos( theta ) + v2*sin( theta );
}
*/
