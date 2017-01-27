/*
 * $Id$
 *
 * Ipseity is a rich-client platform developed in C++ with the Qt
 * framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2013 Ipseity Core Developers
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
 */


#ifndef RLCognitiveSystem_h
#define RLCognitiveSystem_h


#include <QtGui>

#include "AL/IpseityTypes.h"
#include "IpseityString.h"
#include "Math/MathToolBox_Misc.h"
#include "Math/RandomNumberGenerator.h"

#include "AL/BasePluginCognitiveModule.h"
#include "AL/ResponseModule.h"
#include "AL/AbstractRLBehaviorModule.h"
#include "AL/AbstractLearningModule.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/QFunction.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/VFunction.h"
#include "AL/RLCognitiveSystem/Policy/FiniteGreedyPolicy.h"
#include "AL/RLCognitiveSystem/Policy/AbstractContinuousPolicy.h"
#include "AL/RLCognitiveSystem/Policy/BinaryDecisionSequence.h"

#include "IO/PluginWrapper.h"

#include "AL/PerformanceStats.h"
#include "AL/RLCognitiveSystem/RLTypes.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterDoubleSpinBox.h"

#include <fstream>


//! RLCognitiveSystem.
/*!
    A cognitive system consists of:
    - a response module (AbstractResponseModule)
    - a behavior module (AbstractRLBehaviorModule)
    - a learning module (AbstractLearningModule)
    - a Q Function (QFunction)
    \author Fabrice LAURI
*/

class RLCognitiveModule : public QObject, public BasePluginCognitiveModule
{
    Q_OBJECT

    public:
        RLCognitiveModule();
        ~RLCognitiveModule();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_BASE_PLUGIN_COGNITIVE_MODULE

    protected:
        bool canLearn() const;
        void onBeforeSelect();
        void learn();

        void deleteWidgets();


    protected:
        std::map<uint32,SensorySignalProperties*> m_StimulusDescription;
        ResponseSignalProperties m_ResponseDescription;

        Stimulus m_CurrentStimulus;
        Stimulus m_NextStimulus;
        Response m_CurrentResponse;
        Response m_NextResponse;

        PluginWrapper<AbstractRLBehaviorModule> m_PcBehaviorModule;
        AbstractRLBehaviorModule* m_BehaviorModule;

        PluginWrapper<AbstractLearningModule> m_PcLearningModule;
        AbstractLearningModule* m_LearningModule;

        ResponseModule m_ResponseModule;

        QFunction* m_QFunction;
        VFunction* m_VFunction;

        FiniteGreedyPolicy* m_FiniteGreedyPolicy;

        bool m_OnPolicy;


        uint32 m_DataChannelId;
        uint32 m_RewardChannelId;

        bool m_BestResponse;
        Ipseity::Scalar m_DiscountedReturn;
        Ipseity::Scalar m_Return;
        uint32 m_NbrStimuli;

        bool m_KeepPerformance;
        PerformanceStats m_Performance;
        bool m_GenerateFigures;

        std::fstream m_FileEpisodeDataStimuliResponses;

        // GUI
        QGridLayout* m_Layout;
        ParameterCheckBox* m_CheckboxLearning;
        ParameterCheckBox* m_CheckboxUseGreedyPolicy;
        ParameterCheckBox* m_CheckboxSaveEpisodeData;
        ParameterDoubleSpinBox* m_DoubleSpinboxGamma;
        QGroupBox* m_GroupboxBehaviorModule;
        QGroupBox* m_GroupboxMemory;
        QGroupBox* m_GroupboxLearningModule;
};


#endif
