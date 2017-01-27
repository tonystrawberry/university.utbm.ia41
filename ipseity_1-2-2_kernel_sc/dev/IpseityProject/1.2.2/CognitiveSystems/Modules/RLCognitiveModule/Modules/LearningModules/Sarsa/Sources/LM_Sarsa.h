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


#ifndef LearningModule_Sarsa_h
#define LearningModule_Sarsa_h


#include "AL/AbstractLearningModule.h"
#include "AL/RLCognitiveSystem/LearningModules/LM_QValued.h"
#include "AL/RLCognitiveSystem/LearningModules/LM_VIBased.h"


class LearningModule_Sarsa : public LearningModule_QValued, public LearningModule_VIBased
{
    public:
        const char* getBaseName() const;
        const char* getDescription() const;
        const char* getAuthors() const;

        bool isOnlinePolicy() const;
        void learn( const Stimulus & current_stimulus, const Response & current_response, bool is_current_response_best, const Stimulus & next_stimulus_and_reward );
        void learn( const Stimulus & current_stimulus, const Response & current_response, const Stimulus & next_stimulus_and_reward, const Response & next_response );
};


#endif
