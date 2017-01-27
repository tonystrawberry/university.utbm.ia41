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


#include "ParameterDoubleSpinBox.h"


#include <iostream>

using namespace std;


ParameterDoubleSpinBox::ParameterDoubleSpinBox( const char* name, const char* caption ) :
    AbstractParameter( name )
{
    m_Label = new QLabel( caption );
    m_Label->setToolTip( name );
    setToolTip( name );
    m_ValueString = new char [128];

    setFixedSize( 120, 30 );
}


ParameterDoubleSpinBox::~ParameterDoubleSpinBox()
{
    saveValue();
    delete m_Label;
}


void
ParameterDoubleSpinBox::initValue()
{
    QDoubleSpinBox::setValue( m_Settings->value( m_Name.c_str() ).toDouble() );
}


void
ParameterDoubleSpinBox::saveValue()
{
    if (m_Settings)
    {
//        cout << "save " << m_Name.c_str() << "(" << m_Settings->group().toStdString().c_str() << ")" << endl;
        m_Settings->setValue( m_Name.c_str(), value() );
    }
}


const char*
ParameterDoubleSpinBox::toString()
{
    sprintf( m_ValueString, "%.3f", value() );

    return m_ValueString;
}


void
ParameterDoubleSpinBox::setEnabled( bool flag )
{
    QDoubleSpinBox::setEnabled( flag );
    m_Label->setEnabled( flag );
}


void
ParameterDoubleSpinBox::addToLayout( QGridLayout* layout, int row, int column, bool caption_before_value )
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
ParameterDoubleSpinBox::setValue( double value )
{
    QDoubleSpinBox::setValue( value );
}


void
ParameterDoubleSpinBox::setValue( const char* value )
{
    QDoubleSpinBox::setValue( atof( value ) );
}
