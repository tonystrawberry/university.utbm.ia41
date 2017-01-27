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


#include "FiniteGreedyPolicy.h"

#include "Math/RandomNumberGenerator.h"


FiniteGreedyPolicy::FiniteGreedyPolicy( AbstractQFunction* qvm, AbstractResponseModule* rm )
{
    m_QFunction = qvm;
    m_ResponseModule = rm;
}


const Response &
FiniteGreedyPolicy::selectBestResponse( const SensorySignal & x )  // solve \pi(x) = argmax_{u \in U} Q(x,u) when U is a finite space
{
    vector<Response> best_responses;
    int32 nb;
    Ipseity::Scalar q;
    Ipseity::Scalar qmax;

    m_Response = m_ResponseModule->selectFirstResponse( x );
    qmax = m_QFunction->getValue( x, m_Response );
    best_responses.push_back( m_Response );
//    cout << " > " << m_Response << ": " << qmax << endl;
    while( m_ResponseModule->selectNextResponse( x, m_Response ) )
    {
        q = m_QFunction->getValue( x, m_Response );

//        cout << " > " << m_Response << ": " << q << endl;

        if (q > qmax)
        {
            qmax = q;
            best_responses.clear();
            best_responses.push_back( m_Response );
        }
        else if (q == qmax)
        {
            best_responses.push_back( m_Response );
        }
    }

    nb = (int32) best_responses.size();

    int32 nr = 0;
    if (nb > 1)
    {
        nr = RandomNumberGenerator::getRandomNumber( nb );
    }

    m_Response = best_responses[nr];

//    cout << "Best: " << m_Response << "(" << nb << " best responses)" << endl;

    return m_Response;
}


const Response &
FiniteGreedyPolicy::selectRandomResponse( const SensorySignal & x )
{
    return m_ResponseModule->selectRandomResponse( x );
}
