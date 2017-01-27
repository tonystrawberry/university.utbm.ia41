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


#include "LM_Sarsa.h"

#include "AL/RLCognitiveSystem/RLTypes.h"


const char*
LearningModule_Sarsa::getBaseName() const
{
    return "Sarsa";
}


const char*
LearningModule_Sarsa::getDescription() const
{
    return "Online model-free learning technique";
}


const char*
LearningModule_Sarsa::getAuthors() const
{
    return IPSEITY_CREATOR;
}


bool
LearningModule_Sarsa::isOnlinePolicy() const
{
    return true;
}


void
LearningModule_Sarsa::learn( const Stimulus &, const Response &, bool, const Stimulus & )
{
}


void
LearningModule_Sarsa::learn( const Stimulus & x1, const Response & u1, const Stimulus & x2, const Response & u2 )
{
    if (m_QFunction)
    {
        Ipseity::Scalar q1;
        Ipseity::Scalar q2;

        q2 = m_QFunction->getValue( x2.getSensoryData( m_DataChannelId ), u2 );
        q1 = m_QFunction->getValue( x1.getSensoryData( m_DataChannelId ), u1 );

        m_QFunction->decayEligibilityTraces( m_Gamma*m_SpinboxLambda->value() );
        if (m_ButtonAccumulateTraces->isChecked())
        {
            m_QFunction->accumulateEligibilityTraces( x1.getSensoryData( m_DataChannelId ), u1 );
        }
        else
        {
            m_QFunction->replaceEligibilityTraces( x1.getSensoryData( m_DataChannelId ), u1 );
        }

        Ipseity::Scalar reward;

        reward = x2.getSensoryData( m_RewardChannelId )[m_RewardComponentId];

//        cout << "R: " << reward << endl;

        m_QFunction->update( x1.getSensoryData( m_DataChannelId ), u1, q1, reward+m_Gamma*q2 );
    }
}
