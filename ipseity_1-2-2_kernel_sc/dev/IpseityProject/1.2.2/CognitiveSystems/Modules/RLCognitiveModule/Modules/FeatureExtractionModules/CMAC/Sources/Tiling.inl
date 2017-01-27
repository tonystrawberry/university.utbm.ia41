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


#include "Tiling.h"


#include "Math/RandomNumberGenerator.h"


template <class T>
TilingDimension<T>::TilingDimension( uint32 var_id, ScalarDomain<T>* var_property, uint32 nbr_intervals )
{
    VariableId = var_id;
    VariableProperty = var_property;
    NbrIntervals = nbr_intervals;
    Resolution = (var_property->getMax()-var_property->getMin()) / (nbr_intervals-1);
    Offset = RandomNumberGenerator::getRandomNumber( (float64) (getNominalOffset()+Resolution*(NbrIntervals-1))/NbrIntervals );
}


template <class T>
uint32
TilingDimension<T>::getTile( T v )
{
    T tmp = (v <= VariableProperty->getMax()) ? (v >= VariableProperty->getMin() ? v : VariableProperty->getMin()) : VariableProperty->getMax();

    return (uint32) ((tmp-VariableProperty->getMin()+Offset)/Resolution);
}


template <class T>
T
TilingDimension<T>::getNominalOffset()
{
    return (Resolution / (T) 2.0);
}


template <class T>
bool
TilingDimension<T>::load( FILE* file )
{
    fread( &NbrIntervals, sizeof( uint32 ), 1, file );
    fread( &Resolution, sizeof( T ), 1, file );
    fread( &Offset, sizeof( T ), 1, file );

    return true;
}


template <class T>
bool
TilingDimension<T>::save( FILE* file )
{
    fwrite( &NbrIntervals, sizeof( uint32 ), 1, file );
    fwrite( &Resolution, sizeof( T ), 1, file );
    fwrite( &Offset, sizeof( T ), 1, file );

    return true;
}


template <class T>
std::ostream &
operator<<( std::ostream & os, const TilingDimension<T> & td )
{
    return os << "  Id,NbrIntervals,Offset: " << td.VariableId << "," << td.NbrIntervals << "," << td.Offset << std::endl;
}


//***********************************************************************************************************


template <class TPrecision>
Tiling<TPrecision>::Tiling() :
    m_TilesCount( 1 )
{
}


template <class TPrecision>
uint32
Tiling<TPrecision>::getNbrTiles() const
{
    return m_TilesCount;
}


template <class TPrecision>
bool
Tiling<TPrecision>::addDimension( TilingDimension<TPrecision> tiling_dim )
{
    m_TileDimensions.push_back( tiling_dim );
    m_TilesCount *= tiling_dim.NbrIntervals;

    return true;
}


template <class TPrecision>
bool
Tiling<TPrecision>::query( const Vector<TPrecision> & av, uint32 & index )
{
    uint32 vnr;

    index = 0;
    for( uint32 k = 0; k < m_TileDimensions.size(); ++k )
    {
        index *= m_TileDimensions[k].NbrIntervals;
        vnr = m_TileDimensions[k].VariableId;
//		cout << k << ": " << vnr << " = " << av[vnr] << endl;
        index += m_TileDimensions[k].getTile( av[vnr] );
    }

    if (index < m_TilesCount)
    {
        return true;
    }
    else
    {
//		cout << "Tiling Oops: " << index << endl; //Tile #%d / %d\n", ind, m_TilesCount );

        return false;
    }
}


template <class TPrecision>
void
Tiling<TPrecision>::copyFrom( const Tiling & tiling)
{
    m_TilesCount = 1;
    m_TileDimensions.clear();
    for( uint32 k = 0; k < tiling.m_TileDimensions.size(); ++k )
    {
        addDimension( TilingDimension<TPrecision>( tiling.m_TileDimensions[k].VariableId, tiling.m_TileDimensions[k].VariableProperty, tiling.m_TileDimensions[k].NbrIntervals ) );
    }
}


template <class TPrecision>
bool
Tiling<TPrecision>::load( FILE* file )
{
    for( uint32 k = 0; k < m_TileDimensions.size(); k++ )
    {
        m_TileDimensions[k].load( file );
    }

    return true;
}


template <class TPrecision>
bool
Tiling<TPrecision>::save( FILE* file )
{
    for( uint32 k = 0; k < m_TileDimensions.size(); k++ )
    {
        m_TileDimensions[k].save( file );
    }

    return true;
}


template <class TPrecision>
std::ostream &
operator<<( std::ostream & os, const Tiling<TPrecision> & t )
{
    os << "# tiles: " << t.m_TilesCount << std::endl;

    for( uint32 k = 0; k < t.m_TileDimensions.size(); k++ )
    {
        os << t.m_TileDimensions[k];
    }

    return os;
}
