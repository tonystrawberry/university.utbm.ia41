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


#ifndef LearningModule_VIBased_h
#define LearningModule_VIBased_h


#include "AL/AbstractLearningModule.h"
#include "IO/ParameterDoubleSpinBox.h"

#include <QtGui>


class LearningModule_VIBased : public QObject, virtual public AbstractLearningModule
{
    public:
        LearningModule_VIBased();
        ~LearningModule_VIBased();

        void defineParameters();
        bool attachView( uint32 view_id, QWidget* widget );

        bool loadParameters( const char*, const char* );

    protected:
        void deleteWidgets();

    protected:
        uint32 m_RewardComponentId;
        bool m_AccumulateEligibilities;
        Ipseity::Scalar m_Lambda;

    protected:
        QGridLayout* m_Layout;
        QButtonGroup* m_ButtonGroup;
        QRadioButton* m_ButtonAccumulateTraces;
        QRadioButton* m_ButtonReplaceTraces;
        ParameterDoubleSpinBox* m_SpinboxLambda;
};


#endif
