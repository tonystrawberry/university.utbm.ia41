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


#include "LRTA.h"


template <class X, class A>
LRTA<X,A>::LRTA() :
    m_K( 1 )
{
}

template <class X, class A>
LRTA<X,A>::~LRTA()
{
}


template <class X, class A>
void
LRTA<X,A>::setK( uint32 k )
{
    m_K = k;
}


template <class X, class A>
void
LRTA<X,A>::beginThinking()
{
    m_State = LRTA_BEGIN_TRIAL;
}


template <class X, class A>
bool
LRTA<X,A>::think( const X & s )
{
    switch (m_State)
    {
        case LRTA_BEGIN_TRIAL:
            beginTrial( s );

            m_State = LRTA_NEXT_TRIAL;
            return think( s );

        case LRTA_NEXT_TRIAL:

            break;
    }

//    do
//    {
//        m_OldHeuristicFunction = m_HeuristicFunction;
//        trial( s, k );
//    }
//    while (heuristicChanges());
}


template <class X, class A>
void
LRTA<X,A>::uninterruptedThinking( const X & s )
{
    do
    {
        beginTrial( s );
        while( nextTrial( a ) );
    }
    while (heuristicChanges());
}


//*****************************************************************************


template <class X, class A>
void
LRTA<X,A>::beginTrial( const X & s )
{
    m_OldHeuristicFunction = m_HeuristicFunction;

    m_Current = s;
    m_Path.clear();
    m_Path.push_back( s );
}


template <class X, class A>
bool
LRTA<X,A>::nextTrial( A & a )
{
    if (!isGoal( m_Current ))
    {
        X y;

        lookaheadUpdateK( m_Current, m_K, m_Path );
        y = argmin( m_Current, a );
        m_Path.push_back( y );
        m_Current = y;

        return true;
    }
    else
    {
        return false;
    }
}


template <class X, class A>
void
LRTA<X,A>::lookaheadUpdateK( const X & x, uint32 k, const std::list<X> & path )
{
    std::list<X> Q;
    uint32 cont;
    X v;
    X w;

    Q.push_back( x );
    cont = k-1;

    while (Q.size())
    {
        v = Q.pop_front();

        if (lookaheadUpdate1( v ))
        {
            A a;

            a = first_action( v );
            do // foreach a in v
            {
                w = succ( x, a );
                if (find( w, path ) && cont)
                {
                    Q.push_back( w );
                    --cont;
                }
            }
            while( next_action( v, a ) );
        }
    }
}


template <class X, class A>
bool
LRTA<X,A>::lookaheadUpdate1( const X & x )
{
    A a;

    if (argmin( x, a ))
    {
        float64 new_val;

        new_val = c( x,a ) + h( succ( x,a ) );

        if (h( x ) < new_val)
        {
            m_HeuristicFunction[x] = new_val;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


template <class X, class A>
float64
LRTA<X,A>::h( const X & x )
{
    if (m_HeuristicFunction.find( x ) == m_HeuristicFunction.end())
    {
        return (m_HeuristicFunction[x] = h0( x ));
    }
    else
    {
        return m_HeuristicFunction[x];
    }
}


template <class X, class A>
bool
LRTA<X,A>::heuristicChanges() const
{
    if (m_OldHeuristicFunction.size() != m_HeuristicFunction.size())
    {
        return true;
    }
    else
    {
        std::map<X,float64>::const_iterator itr1;
        std::map<X,float64>::const_iterator itr2;

        for( itr1 = m_HeuristicFunction.begin(), itr2 = m_OldHeuristicFunction.begin();
             itr1 != m_HeuristicFunction.end(), itr2 != m_OldHeuristicFunction.end();
             ++itr1, ++itr2 )
        {
            if ((*itr1).first != (*itr2).first ||
                (*itr1).second != (*itr2).second)
            {
                return true;
            }
        }

        return false;
    }
}


template <class X, class A>
bool
LRTA<X,A>::find( const X & x, const std::list<X> & path ) const
{
    std::list<X>::const_iterator itr;

    for( itr = path.begin(); itr != path.end(); ++itr )
    {
        if ((*itr) == x)
        {
            return true;
        }
    }

    return false;
}
