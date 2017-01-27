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


#include "LM_QValued.h"


void
LearningModule_QValued::init( uint32 data_channel_id, uint32 reward_channel_id, Ipseity::Scalar gamma, AbstractVFunction* vvm, AbstractQFunction* qvm )
{
	m_DataChannelId = data_channel_id;
	m_RewardChannelId = reward_channel_id;
	m_Gamma = gamma;
    m_VFunction = vvm;
    m_QFunction = qvm;
}
