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


#include "IpseityString.h"
#include "IO/ParameterCheckBox.h"

#include "MemDefns.h"

#include <iostream>

using namespace std;


ParameterCheckBox::ParameterCheckBox( const char* name, const char* caption, const char* shortcut ) :
    AbstractParameter( name ),
    QCheckBox( caption )
{
    Ipseity::String text;

    text = name;

    if (shortcut)
    {
        setShortcut( QKeySequence(tr( shortcut )) );
        text += "\n";
        text += "(";
        text += shortcut;
        text += ")";
    }

    setToolTip( text.c_str() );
    m_ValueString = new char [2];
    m_ValueString[1] = '\0';
}


ParameterCheckBox::~ParameterCheckBox()
{
    saveValue();
}


void
ParameterCheckBox::initValue()
{
    setChecked( m_Settings->value( m_Name.c_str() ).toBool() );
}


void
ParameterCheckBox::saveValue()
{
    if (m_Settings)
    {
//        cout << "Save " << m_Name.c_str() << "..." << endl;
        m_Settings->setValue( m_Name.c_str(), isChecked() );
    }
}


const char*
ParameterCheckBox::toString()
{
    m_ValueString[0] = '0' + isChecked();

    return m_ValueString;
}


void
ParameterCheckBox::addToLayout( QGridLayout* layout, int row, int column, bool )
{
    layout->addWidget( this, row, column );
}


void
ParameterCheckBox::setValue( bool value )
{
    QCheckBox::setChecked( value );
}


void
ParameterCheckBox::setValue( const char* value )
{
    QCheckBox::setChecked( (atoi( value ) != 0) );
}
