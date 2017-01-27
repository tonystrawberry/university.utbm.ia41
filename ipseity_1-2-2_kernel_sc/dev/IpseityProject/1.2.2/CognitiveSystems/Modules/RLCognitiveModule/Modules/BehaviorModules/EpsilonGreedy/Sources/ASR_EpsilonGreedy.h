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


#ifndef ASR_EpsilonGreedy_h
#define ASR_EpsilonGreedy_h


#include <QtGui>

#include "Types.h"
#include "AL/AbstractRLBehaviorModule.h"
#include "AL/AbstractQFunction.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterDoubleSpinBox.h"


class ASR_EpsilonGreedy : public QObject, public AbstractRLBehaviorModule
{
    Q_OBJECT

    public:
        ASR_EpsilonGreedy();
        ~ASR_EpsilonGreedy();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_RL_BEHAVIOR_MODULE

    private slots:
        void onToggleCheckboxFixedEpsilon( bool );

    protected:
        void setEpsilon( Ipseity::Scalar );

        void deleteWidgets();


    protected:
        AbstractPolicy* m_Policy;
		AbstractQFunction* m_QFunction;
        bool m_FixedEpsilon;
        Ipseity::Scalar m_Epsilon;
        bool m_BestResponseSelected;

        uint32 m_DataChannelId;

        // Gui
        QGridLayout* m_Layout;
        ParameterCheckBox* m_CheckBoxFixedEpsilon;
        ParameterDoubleSpinBox* m_SpinboxEpsilon;
};


#endif
