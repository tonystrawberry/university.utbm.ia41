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


#include "WidgetAgentsList.h"

#include <QDropEvent>
#include <QDebug>
#include <QApplication>

#include "MemDefns.h"


WidgetAgentsList::WidgetAgentsList()
{
    setColumnCount( 3 );
    setColumnWidth( 0, 80 );
    setColumnWidth( 1, 160 );
    setColumnWidth( 2, 160 );

    QStringList labels;
    labels << "Id" << "Name" << "Taxon Name";
    setHeaderLabels( labels );

//    setSelectionMode( QAbstractItemView::ExtendedSelection );
}

WidgetAgentsList::~WidgetAgentsList()
{
    clear();
}


void
WidgetAgentsList::clear()
{
    for( uint32 id = 0; id < m_Items.size(); ++id )
    {
        delete m_Items[id];
    }
    m_Items.clear();

    QTreeWidget::clear();
}


void
WidgetAgentsList::addListItem( uint32 agent_id, const QString & agent_name, const QString & taxon_name )
{
    m_Item = new WidgetAgentItem( agent_id, agent_name, taxon_name, this );
    this->addTopLevelItem( m_Item );

    m_Items.push_back( m_Item );

    if (m_Items.size() == 1)
    {
        this->setItemSelected( m_Item, true );
    }
}
