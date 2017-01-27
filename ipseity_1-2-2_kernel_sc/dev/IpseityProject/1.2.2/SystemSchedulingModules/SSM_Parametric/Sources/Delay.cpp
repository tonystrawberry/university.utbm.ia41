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


#include "Delay.h"

#include "Math/RandomNumberGenerator.h"


const float64 MAX_DELAY = 100000.0;


Delay::Delay( QWidget* widget )
{
    m_Layout = new QGridLayout( widget );
    m_Layout->setAlignment( Qt::AlignTop );

    m_LabelMean = new QLabel( "Mean:" );
    m_MeanDelay = new QDoubleSpinBox();
    m_MeanDelay->setRange( 0.0, MAX_DELAY );
    m_MeanDelay->setFixedHeight( 30 );

    m_LabelInterval = new QLabel( "Interval:" );
    m_IntervalDelay = new QDoubleSpinBox();
    m_IntervalDelay->setRange( 0.0, MAX_DELAY );
    m_IntervalDelay->setFixedHeight( 30 );

    m_Layout->addWidget( m_LabelMean, 0,0,1,1 );
    m_Layout->addWidget( m_MeanDelay, 0,1,1,1 );
    m_Layout->addWidget( m_LabelInterval, 1,0,1,1 );
    m_Layout->addWidget( m_IntervalDelay, 1,1,1,1 );
}


Delay::~Delay()
{
    delete m_Layout;
    delete m_LabelMean;
    delete m_MeanDelay;
    delete m_LabelInterval;
    delete m_IntervalDelay;
}


float64
Delay::getMin()
{
    return m_MeanDelay->value()-m_IntervalDelay->value();
}


float64
Delay::getMax()
{
    return m_MeanDelay->value()+m_IntervalDelay->value();
}


float64
Delay::getRandomValue()
{
    return RandomNumberGenerator::getRandomNumber( getMin(), getMax() );
}


DelayTrigger::DelayTrigger( Delay* delay ) :
    m_Delay( delay ),
    m_ElapsedTime( 0.0 )
{
}


bool
DelayTrigger::timeout( float64 elapsed_time )
{
    if (m_ElapsedTime == 0.0)
    {
        m_RandomDelay = m_Delay->getRandomValue();
    }

    m_ElapsedTime += elapsed_time;

    if (m_ElapsedTime >= m_RandomDelay)
    {
        m_ElapsedTime = 0.0;

//        cout << "> " << m_RandomDelay << endl;

        return true;
    }
    else
    {
        return false;
    }
}
