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


#ifndef Tiling_h
#define Tiling_h


#include "Types.h"
#include "Math/Vector.h"

#include <vector>


template <class T>
class TilingDimension;

template <class T>
std::ostream & operator<<( std::ostream &, const TilingDimension<T> & );


template <class T>
class TilingDimension
{
    public:
        TilingDimension( uint32 var_id, ScalarDomain<T>* var_property, uint32 nbr_intervals );

        uint32 getTile( T );

        bool load( FILE* );
        bool save( FILE* );

        friend std::ostream & operator<< <>( std::ostream &, const TilingDimension<T> & );

    protected:
        T getNominalOffset();


    public:
        uint32 VariableId;
        ScalarDomain<T>* VariableProperty;
        uint32 NbrIntervals;
        T Resolution;
        T Offset;
};


template <class TPrecision>
class Tiling;

template <class TPrecision>
std::ostream & operator<<( std::ostream & os, const Tiling<TPrecision> & v );


template <class TPrecision>
class Tiling
{
    public:
        Tiling();

        uint32 getNbrTiles() const;

        bool addDimension( TilingDimension<TPrecision> );

        bool query( const Vector<TPrecision> &, uint32 & );

        void copyFrom( const Tiling<TPrecision> & tiling );

        bool load( FILE* );
        bool save( FILE* );

        friend std::ostream & operator<< <>( std::ostream & os, const Tiling<TPrecision> & v );


    protected:
        std::vector<TilingDimension<TPrecision> > m_TileDimensions;
        uint32 m_TilesCount;
};


#include "Tiling.inl"


#endif
