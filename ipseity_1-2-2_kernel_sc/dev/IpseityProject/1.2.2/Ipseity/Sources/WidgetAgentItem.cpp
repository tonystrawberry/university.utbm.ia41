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


#include "WidgetAgentItem.h"


WidgetAgentItem::WidgetAgentItem( uint32 agent_id, const QString & agent_name, const QString & taxon_name, QTreeWidget* _parent ) : QTreeWidgetItem(_parent)
{
    this->setSizeHint(0,QSize(80,30));
    this->setSizeHint(1,QSize(160,30));
    this->setSizeHint(2,QSize(160,30));

    m_AgentId = new QLabel( tr("%1").arg(agent_id) );
    m_AgentName = new QLabel( agent_name );
    m_TaxonName = new QLabel( taxon_name);

    if (_parent != NULL)
    {
        this->treeWidget()->setItemWidget( this, 0, m_AgentId );
        this->treeWidget()->setItemWidget( this, 1, m_AgentName );
        this->treeWidget()->setItemWidget( this, 2, m_TaxonName );
    }
}


WidgetAgentItem::~WidgetAgentItem()
{
    delete m_AgentId;
    delete m_AgentName;
    delete m_TaxonName;
}
