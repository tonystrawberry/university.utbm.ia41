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


#ifndef __MEMDEFNS__
#define __MEMDEFNS__


#include <string.h>


//! Functions for managing memory allocation.
/*!
 *  \author Fabrice LAURI
*/


#define SafeDelete( x )			if( x ) { delete x; x = NULL; }
#define SafeRelease( x )        if( x ) { x->Release(); x = NULL; }

#define SafeDeleteArray( x )	if( x ) { delete [] x; x = NULL; }
#define SafeReleaseArray( x, w )\
if( x )\
{\
	for( int i = 0; i < w; i++ )\
	{\
		if( x[i] )\
		{\
			x[i]->Release();\
		}\
	}\
	x = NULL;\
}

#define SafeDeleteRefsArray( x, w )\
if( x )\
{\
	for( int i = 0; i < w; i++ )\
	{\
	    if( x[i] ) delete x[i];\
	}\
	delete [] x;\
	x = NULL;\
}

#define SafeReleaseDevice( x )  if( x ) { x->Unacquire(); x->Release(); x = NULL; }

#define SafeDelete2DArray( x, w )\
if( x )\
{\
	for( int i = 0; i < w; i++ )\
	{\
		delete [] x[i];\
	}\
	delete [] x;\
	x = NULL;\
}

#define SafeRelease2DArray( x, w, h )\
if( x )\
{\
	int i, j;\
	for( j = 0; j < h; j++ )\
	{\
		for( i = 0; i < w; i++ )\
		{\
			if( x[i][j] )\
				x[i][j]->Release();\
		}\
	}\
	x = NULL;\
}\


#define CreateSafeArray( x, t, n ) \
	SafeDeleteArray( x ); \
	x = new t [n];


#define FillArray( x, n, v )\
{\
	int i;\
	for( i = 0; i < n; i++ )\
	{\
		x[i] = v;\
	}\
}


#define Create2DArray( x, w, h, t )\
x = new t * [w];\
{\
	int i;\
    for( i = 0; i < (int) w; i++ )\
	{\
		x[i] = new t [h];\
	}\
}


#define Fill2DArray( x, w, h, v )\
{\
	int i, j;\
	for( j = 0; j < h; j++ )\
	{\
		for( i = 0; i < w; i++ )\
		{\
			x[i][j] = v;\
		}\
	}\
}


#define Create3DArray( x, n1, n2, n3, t )\
x = new t ** [n1];\
{\
	int i, j;\
    for( i = 0; i < n1; i++ )\
	{\
		x[i] = new t * [n2];\
		for( j = 0; j < n2; j++ )\
		{\
			x[i][j] = new t [n3];\
		}\
	}\
}


#define Fill3DArray( x, n1, n2, n3, v )\
{\
	int i, j, k;\
	for( k = 0; k < n3; k++ )\
	{\
		for( j = 0; j < n2; j++ )\
		{\
			for( i = 0; i < n1; i++ )\
			{\
				x[i][j][k] = v;\
			}\
		}\
	}\
}


#define cvI32LE( i32, tab, k ) i32 = ((tab[k+3] << 24)+(tab[k+2] << 16)+(tab[k+1] << 8)+tab[k+0])
#define cvI32BE( i32, tab, k ) i32 = ((tab[k+0] << 24)+(tab[k+1] << 16)+(tab[k+2] << 8)+tab[k+3])

#define cvF32LE( f32, tab, k ) memcpy( &f32, tab+k, sizeof( float ) )
#define cvF64LE( f64, tab, k ) memcpy( &f64, tab+k, sizeof( double ) )


#endif
