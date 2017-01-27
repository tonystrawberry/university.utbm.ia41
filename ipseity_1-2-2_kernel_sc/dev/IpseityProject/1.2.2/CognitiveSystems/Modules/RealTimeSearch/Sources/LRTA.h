/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2014, 2013 Ipseity Core Developers
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


#ifndef LRTA_h
#define LRTA_h


#include "AL/IpseityTypes.h"


#include <map>
#include <list>


enum
{
    LRTA_BEGIN              = 0,
    LRTA_BEGIN_TRIAL,
    LRTA_NEXT_TRIAL,
    LRTA_NEXT_ITR
};


template <class X, class A>
class LRTA
{
    public:
        LRTA();
        ~LRTA();

        void run( const X & s, uint32 k );

    protected:
        void trial( const X & s, uint32 k );
        void lookaheadUpdateK( const X & x, uint32 k, const std::list<X> & path );
        bool lookaheadUpdate1( const X & x );

        float64 h( const X & x );
        bool heuristicChanges() const;

        bool find( const X & x, std::list<X> path ) const;


    protected:
        virtual float64 h0( const X & x ) = 0;
        virtual float64 c( const X & x, const A & a ) = 0;
        virtual const X & succ( const X & x, const A & a ) = 0;
        virtual bool argmin( const X & x, A & a ) = 0;
        virtual bool isGoal( const X & x ) const = 0;


    protected:
        std::map<X,float64> m_OldHeuristicFunction;
        std::map<X,float64> m_HeuristicFunction;
        X m_Current;
        X y;
        std::list<X> m_Path;
        uint32 m_State;
        uint32 m_K;
};


#include "LRTA.inl"


#endif

