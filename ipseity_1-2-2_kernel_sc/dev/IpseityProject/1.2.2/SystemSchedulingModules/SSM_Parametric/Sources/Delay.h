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


#ifndef Delay_h
#define Delay_h


#include "AL/IpseityTypes.h"

#include <QtGui>


class Delay : public QObject
{
    Q_OBJECT

    public:
        Delay( QWidget* widget );
        ~Delay();

        float64 getMin();
        float64 getMax();

        float64 getRandomValue();


    protected:
        // Gui
        QGridLayout* m_Layout;
        QLabel* m_LabelMean;
        QDoubleSpinBox* m_MeanDelay;
        QLabel* m_LabelInterval;
        QDoubleSpinBox* m_IntervalDelay;
};


class DelayTrigger
{
    public:
        DelayTrigger( Delay* delay );

        bool timeout( float64 elapsed_time );

    protected:
        Delay* m_Delay;
        float64 m_ElapsedTime;
        float64 m_RandomDelay;
};


#endif
