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


#include "WidgetOrganizationItem.h"

#include <QLabel>


WidgetOrganizationItem::WidgetOrganizationItem( int org_id, const QString & roles, QTreeWidget* _parent) :
QTreeWidgetItem(_parent)
{
	this->setSizeHint(0,QSize(50,30));
	this->setSizeHint(1,QSize(500,30));

	m_OrganizationId = new QLabel( tr("%1").arg(org_id) );
	m_OrganizationId->setFixedWidth( 50 );

	m_Roles = new QLabel( roles );
	m_Roles->setFixedWidth( 500 );

	if (_parent != NULL)
	{
		this->treeWidget()->setItemWidget( this, 0, m_OrganizationId );
		this->treeWidget()->setItemWidget( this, 1, m_Roles );
	}
}
