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


#include "ControlButton.h"



ControlButton::ControlButton( const char* name, const char* caption, const char* shortcut ) :
    AbstractControl( name )
{
    if (shortcut)
    {
        m_Shortcut = shortcut;
    }

    setToolTip( caption );
    setMinimumSize( 64,64 );
}


ControlButton::~ControlButton()
{
}


void
ControlButton::setToolTip( const QString & caption )
{
    Ipseity::String text;

    text = caption.toStdString().c_str();

    if (m_Shortcut.getLength())
    {
        text += "\n";
        text += "(";
        if (!strcmp( m_Shortcut.c_str(), "Ret" ))
        {
            setShortcut( QKeySequence( Qt::Key_Return ) );
            text += "Return";
        }
        else
        {
            setShortcut( QKeySequence( m_Shortcut.c_str() ) );
            text += m_Shortcut.c_str();
        }
        text += ")";
    }

    QPushButton::setToolTip( text.c_str() );
}
