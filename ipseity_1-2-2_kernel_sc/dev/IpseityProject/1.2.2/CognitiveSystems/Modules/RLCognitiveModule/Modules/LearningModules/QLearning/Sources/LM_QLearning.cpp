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


#include "LM_QLearning.h"

#include "AL/RLCognitiveSystem/RLTypes.h"


const char*
LearningModule_QLearning::getBaseName() const
{
    return "QLearning";
}


const char*
LearningModule_QLearning::getDescription() const
{
    return "Offline model-free learning technique";
}


const char*
LearningModule_QLearning::getAuthors() const
{
    return IPSEITY_CREATOR;
}


bool
LearningModule_QLearning::isOnlinePolicy() const
{
    return false;
}


void
LearningModule_QLearning::learn( const Stimulus & x1, const Response & u, bool is_current_response_best, const Stimulus & x2 )
{
    if (m_VFunction && m_QFunction)
    {
        Ipseity::Scalar q1;
        Ipseity::Scalar q2;

        q2 = m_VFunction->getValue( x2.getSensoryData( m_DataChannelId ) );
        q1 = m_QFunction->getValue( x1.getSensoryData( m_DataChannelId ), u );

        if (is_current_response_best)
        {
            m_QFunction->decayEligibilityTraces( m_Gamma*m_SpinboxLambda->value() );
        }
        else
        {
            m_QFunction->clearEligibilityTraces();
        }

        if (m_ButtonAccumulateTraces->isChecked())
        {
            m_QFunction->accumulateEligibilityTraces( x1.getSensoryData( m_DataChannelId ), u );
        }
        else
        {
            m_QFunction->replaceEligibilityTraces( x1.getSensoryData( m_DataChannelId ), u );
        }

        Ipseity::Scalar reward;

        reward = x2.getSensoryData( m_RewardChannelId )[m_RewardComponentId];

//        cout << "R: " << reward << endl;

        m_QFunction->update( x1.getSensoryData( m_DataChannelId ), u, q1, reward+m_Gamma*q2 );
    }
}


void
LearningModule_QLearning::learn( const Stimulus &, const Response &, const Stimulus &, const Response & )
{
}
