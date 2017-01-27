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


#include "ParameterLineEdit.h"

#include "AbstractPlugin.h"
#include "MemDefns.h"


#include <iostream>

using namespace std;


ParameterLineEdit::ParameterLineEdit( const char* name, const char* caption, QString text ) :
    AbstractParameter( name ),
    QLineEdit( text ),
    m_Label( NULL )
{
    if (strlen( caption ))
    {
        m_Label = new QLabel( caption );
        m_Label->setToolTip( name );
    }
    setToolTip( name );

    m_ValueString = new char [32000];

    setFixedHeight( 30 );
}


ParameterLineEdit::~ParameterLineEdit()
{
    saveValue();
    SafeDelete( m_Label );
}


void
ParameterLineEdit::initValue()
{
    setText( m_Settings->value( m_Name.c_str() ).toString() );
}


void
ParameterLineEdit::saveValue()
{
    if (m_Settings)
    {
    //        cout << "save " << m_Name.c_str() << "(" << m_Settings->group().toStdString().c_str() << ")" << endl;
        m_Settings->setValue( m_Name.c_str(), text() );
    }
}


const char*
ParameterLineEdit::toString()
{
    QStringList list;
    char* src = (char*) text().toStdString().c_str();
    char* dst = m_ValueString;
    int in_token;
    AbstractParameter* param;

    list = text().split( "$", QString::SkipEmptyParts );

    dst[0] = '\0';
    in_token = ((src[0] == '$') ? 1 : 0);

    for( QStringList::iterator itr = list.begin(); itr != list.end(); ++itr )
    {
        if (in_token == 1)
        {
            if ((param = m_Host->findParameter( (*itr).toStdString().c_str() )))
            {
                strcat( dst, param->toString() );
            }
        }
        else
        {
            strcat( dst, (*itr).toStdString().c_str() );
        }

        in_token = 1-in_token;
    }

    return m_ValueString;
}


void
ParameterLineEdit::setEnabled( bool flag )
{
    QLineEdit::setEnabled( flag );
    if (m_Label)
    {
        m_Label->setEnabled( flag );
    }
}


void
ParameterLineEdit::addToLayout( QGridLayout* layout, int row, int column, bool caption_before_value )
{
    if (caption_before_value)
    {
        if (m_Label)
        {
            layout->addWidget( m_Label, row, column );
            layout->addWidget( this, row, column+1 );
        }
        else
        {
            layout->addWidget( this, row, column );
        }
    }
    else
    {
        if (m_Label)
        {
            layout->addWidget( this, row, column );
            layout->addWidget( m_Label, row, column+1 );
        }
        else
        {
            layout->addWidget( this, row, column );
        }
    }
}


void
ParameterLineEdit::setValue( const char* value )
{
    setText( value );
}
