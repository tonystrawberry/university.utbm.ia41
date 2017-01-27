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


#include "WidgetTaxonsList.h"

#include <QDropEvent>
#include <QDebug>
#include <QApplication>

#include "WidgetTaxonItem.h"
#include "MemDefns.h"

#include <iostream>


WidgetTaxonsList::WidgetTaxonsList()
{
    setColumnCount( 5 );
    setColumnWidth( 0, 160 );
    setColumnWidth( 1, 120 );
    setColumnWidth( 2, 120 );
    setColumnWidth( 3, 120 );
    setColumnWidth( 4, 240 );

    QStringList labels;
    labels << "Name" << "Controllable?" << "Supervisor?" << "Shared?" << "Cognitive System";
    setHeaderLabels( labels );

    setSelectionMode( QAbstractItemView::ExtendedSelection );
}

WidgetTaxonsList::~WidgetTaxonsList()
{
    clear();
}


void
WidgetTaxonsList::clear()
{
    for( uint32 taxon_id = 0; taxon_id < m_Items.size(); ++taxon_id )
    {
        delete m_Items[taxon_id];
    }
    m_Items.clear();

    QTreeWidget::clear();
}


void
WidgetTaxonsList::addListItem( const QString & taxon_name, uint32 status )
{
    m_Item = new WidgetTaxonItem( taxon_name, status, this );
    this->addTopLevelItem( m_Item );
    connect( m_Item, SIGNAL(updateTaxonStatusControllable( int, bool )), this, SIGNAL(updateTaxonStatusControllable( int, bool )));
    connect( m_Item, SIGNAL(currentNameIndexChanged(int,int)), this, SIGNAL(currentNameIndexChanged( int, int )));

    m_Items.push_back( m_Item );

    if (m_Items.size() == 1)
    {
        this->setItemSelected( m_Item, true );
    }
}


void
WidgetTaxonsList::addCognitiveSystem( const char* cs_name )
{
    m_Item->addCognitiveSystem( cs_name );
}


void
WidgetTaxonsList::setCognitiveSystemEnabled( uint32 taxon_id, bool f )
{
    m_Items[taxon_id]->setCognitiveSystemNameEnabled( f );
}


void
WidgetTaxonsList::toggleShareSelectedCognitiveSystems()
{
    for( uint32 taxon_id = 0; taxon_id < m_Items.size(); ++taxon_id )
    {
        if (m_Items[taxon_id]->isSelected())
        {
            m_Items[taxon_id]->toggleShareCognitiveSystem();
        }
    }
}


void
WidgetTaxonsList::changeIndex(int taxon_id, int index)
{
    m_Items[taxon_id]->changeIndex( index );
}


void
WidgetTaxonsList::unselectAll()
{
    for( uint32 taxon_id = 0; taxon_id < m_Items.size(); ++taxon_id )
    {
        m_Items[taxon_id]->setSelected( false );
    }
}


void
WidgetTaxonsList::select( int taxon_id )
{
    m_Items[taxon_id]->setSelected( true );
}


uint32
WidgetTaxonsList::getTaxonStatus( uint32 taxon_id ) const
{
    return m_Items[taxon_id]->getStatus();
}
