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


#ifndef RCAL_h
#define RCAL_h


#include <QtGui>

#include "AL/IpseityTypes.h"
#include "Math/RandomNumberGenerator.h"

#include "AL/BasePluginCognitiveModule.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterSpinBox.h"
#include "IO/ParameterDoubleSpinBox.h"
#include "IO/ParameterLineEdit.h"
#include "IO/ParameterSlider.h"

#include "bilal_classifier.h"


//! RCAL learner

class RCAL : public QObject, public BasePluginCognitiveModule
{
    Q_OBJECT

    public:
        RCAL();
        ~RCAL();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_BASE_PLUGIN_COGNITIVE_MODULE

    protected slots:
        void onEditChange();
        void onParamClassifChange();

    protected:
        Stimulus m_CurrentStimulus;
        Response m_NextResponse;

        int n_a;
        int N_base;

        int m_NbrExpertSamples;
        int m_NbrNonExpertSamples;
        int m_NbrTotalSamples;

        int m_ActionVectorDimension;
        int m_StateVectorDimension;

        double* m_MatrixExpertStates;
        int* m_MatrixExpertActions;
        double* m_MatrixExpertNextStates;
        double* m_MatrixNonExpertStates;
        int* m_MatrixNonExpertActions;
        double* m_MatrixNonExpertNextStates;
        bool m_Classify;
        bool m_LoadNewData;

        BilalClassifier* RCAL_classifier;

        // GUI
        QGridLayout* m_Layout;
        ParameterLineEdit* m_LineEditExpertFilename;
        ParameterLineEdit* m_LineEditNonExpertFilename;
        ParameterSpinBox* m_SpinboxNbrTotalSamples;
        ParameterSlider* m_SliderPercentageNonExpertSamples;
        ParameterDoubleSpinBox* m_SpinboxLambda;
        ParameterSpinBox* m_SpinboxNbrLearners;
};


#endif
