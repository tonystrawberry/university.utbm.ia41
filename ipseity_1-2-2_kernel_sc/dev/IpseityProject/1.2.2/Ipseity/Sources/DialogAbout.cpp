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


#include "DialogAbout.h"

#include "Defines.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>


const char* TEXT_ABOUT =
"<b>"
APP_NAME
" "
VERSION
"</b><br>"
"<b>"
"Synthesis Laboratory of Artificial Cognitive Systems"
"<br>"
"in Multi-Agent Systems"
"</b><br>"
DATE
"<br>"
"<br>"
"<b>Core Developers:</b>"
"<br>"
"<b>Fabrice Lauri</b> <i>[1]</i><br>"
"(fabrice.lauri@utbm.fr)<br>"
"<br><br>"
"<b>Status:</b><br>"
"<i>[1]</i> Associate professor at the<br>"
AFFILIATION
"<br><br>"
;


DialogAbout::DialogAbout()
{
    QVBoxLayout* layout = new QVBoxLayout( this );

    QLabel* text = new QLabel();
    text->setAlignment(Qt::AlignCenter);
    text->setText( QString( TEXT_ABOUT ) );
    text->setFixedWidth( 460 );

    QPushButton* button_ok = new QPushButton( "Ok" );
    button_ok->setFixedWidth( 100 );
    connect(button_ok,SIGNAL(clicked()),this,SLOT(close()));

    layout->addWidget( text );
    layout->addWidget( button_ok );
    layout->setAlignment( button_ok, Qt::AlignCenter );
}
