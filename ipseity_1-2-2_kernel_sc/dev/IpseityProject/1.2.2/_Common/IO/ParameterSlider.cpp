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


#include "ParameterSlider.h"


#include <iostream>

using namespace std;


ParameterSlider::ParameterSlider( const char* name, const char* caption, Qt::Orientation orientation, int fixed_width ) :
    AbstractParameter( name ),
    QSlider( orientation )
{
    m_Label = new QLabel( caption );
    m_Caption = caption;
    m_Label->setToolTip( name );
    if (fixed_width > 0)
    {
        m_Label->setFixedWidth( fixed_width );
    }
    setToolTip( name );
    m_ValueString = new char [128];

    setFixedHeight( 30 );

    connect( this, SIGNAL(valueChanged(int)), this, SLOT(onChangeValue(int)) );
}


ParameterSlider::~ParameterSlider()
{
    saveValue();
    delete m_Label;
}


void
ParameterSlider::initValue()
{
    QSlider::setValue( m_Settings->value( m_Name.c_str() ).toInt() );
}


void
ParameterSlider::saveValue()
{
    if (m_Settings)
    {
    //        cout << "save " << m_Name.c_str() << "(" << m_Settings->group().toStdString().c_str() << ")" << endl;
        m_Settings->setValue( m_Name.c_str(), value() );
    }
}


void
ParameterSlider::onChangeValue( int value )
{
    m_Label->setText( QString( m_Caption ).arg( value, 1+((int) log( (double) maximum() )), 10, QLatin1Char( ' ' ) ) );
}


const char*
ParameterSlider::toString()
{
    sprintf( m_ValueString, "%d", value() );

    return m_ValueString;
}


void
ParameterSlider::setEnabled( bool flag )
{
    QSlider::setEnabled( flag );
    m_Label->setEnabled( flag );
}


void
ParameterSlider::addToLayout( QGridLayout* layout, int row, int column, bool caption_before_value )
{
    if (caption_before_value)
    {
        layout->addWidget( m_Label, row, column );
        layout->addWidget( this, row, column+1 );
    }
    else
    {
        layout->addWidget( this, row, column );
        layout->addWidget( m_Label, row, column+1 );
    }
}


void
ParameterSlider::setValue( int value )
{
    QSlider::setValue( value );
}


void
ParameterSlider::setValue( const char* value )
{
    QSlider::setValue( atoi( value ) );
}
