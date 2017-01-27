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


#ifndef ASR_Softmax_h
#define ASR_Softmax_h


#include <QtGui>

#include "Types.h"
#include "AL/AbstractRLBehaviorModule.h"
#include "Math/RouletteWheel.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterDoubleSpinBox.h"


class ASR_Softmax : public QObject, public AbstractRLBehaviorModule
{
    Q_OBJECT

    public:
		ASR_Softmax();
		~ASR_Softmax();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_RL_BEHAVIOR_MODULE

    private slots:
		void onToggleCheckboxFixedTau( bool );

    protected:
		void setTau( Ipseity::Scalar );

        void deleteWidgets();


    protected:
        AbstractPolicy* m_Policy;
		AbstractQFunction* m_QFunction;
		bool m_FixedTau;
		Ipseity::Scalar m_Tau;
        bool m_BestResponseSelected;

		float64* m_Probabilities;
		Response m_Response;
		RouletteWheel<uint32>* m_RouletteWheel;

        uint32 m_DataChannelId;

        // Gui
        QGridLayout* m_Layout;
        ParameterCheckBox* m_CheckBoxFixedTau;
        ParameterDoubleSpinBox* m_SpinboxTau;
};


#endif
