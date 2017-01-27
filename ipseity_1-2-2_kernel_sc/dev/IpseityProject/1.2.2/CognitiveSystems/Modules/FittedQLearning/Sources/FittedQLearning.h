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


#ifndef FittedQLearning_h
#define FittedQLearning_h


#include <QtGui>

#include "AL/IpseityTypes.h"
#include "Math/RandomNumberGenerator.h"

#include "AL/BasePluginCognitiveModule.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterSpinBox.h"
#include "IO/ParameterDoubleSpinBox.h"
#include "IO/ParameterLineEdit.h"
#include "IO/ParameterSlider.h"

#include "TreeEnsemble.h"


//! FittedQ

class FittedQLearning : public QObject, public BasePluginCognitiveModule
{
    Q_OBJECT

    public:
        FittedQLearning();
        ~FittedQLearning();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_BASE_PLUGIN_COGNITIVE_MODULE

    protected slots:
        void onEditChange();
        void onParamChange();

    protected:
        void oneStepVI();
        float64 getV( const SensorySignal & x, uint32 & best_action );
        float64 getValue( const SensorySignal & x, const Response & u );
        void findNewQFunction();


    protected:
        Stimulus m_CurrentStimulus;
        Response m_NextResponse;
        SensorySignal m_State;
        Response m_Response;

        int n_a;
        int N_base;

        int m_NbrExpertSamples;
        int m_NbrNonExpertSamples;
        int m_NbrTotalSamples;

        int m_ActionVectorDimension;
        int m_StateVectorDimension;
        int m_RewardVectorDimension;

        float64* m_MatrixStatesActions;
        float64* m_StatesActionsMins;
        float64* m_StatesActionsMaxs;
        float64* m_MatrixStates;
        int* m_MatrixActions;
        float64* m_MatrixNextStates;
        float64* m_MatrixRewards;
        float64* m_SampleQValues;
        std::vector<uint32> m_ValidEntries;
        bool m_Learn;
        bool m_LoadNewData;
        TreeEnsemble* m_TreeEnsemble;

        float64* m_Input;


        // GUI
        QGridLayout* m_Layout;
        ParameterDoubleSpinBox* m_DoubleSpinboxGamma;
        ParameterSpinBox* m_SpinboxNbrIterations;

        ParameterSpinBox* m_SpinboxNbrTrees;
        ParameterSpinBox* m_SpinboxNbrCuts;
        ParameterSpinBox* m_SpinboxMinSamples;
        ParameterSpinBox* m_SpinboxMaxLevels;

        ParameterLineEdit* m_LineEditExpertFilename;
        ParameterLineEdit* m_LineEditNonExpertFilename;
        ParameterSpinBox* m_SpinboxNbrTotalSamples;
        ParameterSlider* m_SliderPercentageNonExpertSamples;
};


#endif
