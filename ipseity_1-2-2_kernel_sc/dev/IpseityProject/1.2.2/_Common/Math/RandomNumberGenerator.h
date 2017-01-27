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


#ifndef RandomNumberGenerator_h
#define RandomNumberGenerator_h


#include "Types.h"


class RandomNumberGenerator
{
    public:
        static void initSeed( uint32 );


        // generates integer numbers in the [0, max)
        static int32 getRandomNumber( int32 max )
        {
            return rand_int32() % max;
        }

        // generates integer numbers in the [min, max)
        static int32 getRandomNumber( int32 min, int32 max )
        {
            return min+(rand_int32() % (max-min));
        }


        // generates floating point numbers in the [0, 1]
        static float32 getFloat32Closed()
        {
            return static_cast<float32>(rand_int32()) * (1.0f / 4294967295.0f); // divided by 2^32
        }

        // generates floating point numbers in the (0, 1)
        static float32 getFloat32Open()
        {
            return (static_cast<float32>(rand_int32())+0.5f) * (1.0f / 4294967296.0f); // divided by 2^32
        }

        // generates floating point numbers in the [0, 1)
        static float32 getFloat32ClosedOpen()
        {
            return static_cast<float32>(rand_int32()) * (1.0f / 4294967296.0f); // divided by 2^32
        }

        // generates floating point numbers in the (0, 1]
        static float32 getFloat32OpenClosed()
        {
            return (static_cast<float32>(rand_int32())+0.5f) * (1.0f / 4294967295.0f); // divided by 2^32
        }

        // generates floating point numbers in the [0,max)
        static float32 getRandomNumber( float32 max )
        {
            return getFloat32ClosedOpen()*max;
        }

        // generates floating point numbers in the [min,max)
        static float32 getRandomNumber( float32 min, float32 max )
        {
            return min+(getFloat32ClosedOpen()*(max-min));
        }


        // generates floating point numbers in the [0, 1]
        static float64 getFloat64Closed()
        {
            return static_cast<float64>(rand_int32()) * (1.0 / 4294967295.0); // divided by 2^32
        }

        // generates double floating point numbers in (0, 1)
        static float64 getFloat64Open()
        {
            return (static_cast<float64>(rand_int32())+0.5) * (1.0 / 4294967296.0); // divided by 2^32
        }

        // generates double floating point numbers in [0, 1)
        static float64 getFloat64ClosedOpen()
        {
            return static_cast<float64>(rand_int32()) * (1.0 / 4294967296.0); // divided by 2^32
        }

        // generates double floating point numbers in (0, 1]
        static float64 getFloat64OpenClosed()
        {
            return (static_cast<float64>(rand_int32())+0.5) * (1.0 / 4294967295.0); // divided by 2^32
        }

        // generates floating point numbers in the [0,max)
        static float64 getRandomNumber( float64 max )
        {
            return getFloat64ClosedOpen()*max;
        }

        // generates floating point numbers in the [min,max)
        static float64 getRandomNumber( float64 min, float64 max )
        {
            return min+(getFloat64ClosedOpen()*(max-min));
        }


        static float32 getFloat32Normal();
        static float32 getFloat32Exp();


    protected: // used by derived classes, otherwise not accessible; use the ()-operator
        static uint32 rand_int32(); // generate 32 bit random integer

    private:
        static void zigset( uint32 );

    private:
		static bool m_Initialized;

        static const int32 n = 624, m = 397; // compile time constants
        // the variables below are static (no duplicates can exist)
        static uint32 state[n]; // state vector array
        static int32 p; // position in state array

        // private functions used to generate the pseudo random numbers
        static uint32 twiddle( uint32, uint32 ); // used by gen_state()
        static void gen_state(); // generate new state
};


#endif
