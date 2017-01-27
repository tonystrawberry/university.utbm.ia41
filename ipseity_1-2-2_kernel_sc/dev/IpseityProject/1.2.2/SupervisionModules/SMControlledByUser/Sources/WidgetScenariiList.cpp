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


#include "WidgetScenariiList.h"

#include <QDropEvent>
#include <QDebug>
#include <QApplication>
#include "WidgetScenarioItem.h"


const int cstScenarioNameColumnWidth = 260;
const int cstScenarioNbrEpisodes = 120;
const int cstScenarioLearningEpisodes = 120;
const int cstScenarioTestEpisodes = 120;
const int cstScenarioProgressColumnWidth = 160;
const int cstScenarioIterations = 60;


WidgetScenariiList::WidgetScenariiList()
{
    setColumnCount( 6 );
    setColumnWidth( 0, cstScenarioNameColumnWidth );
    setColumnWidth( 1, cstScenarioNbrEpisodes );
    setColumnWidth( 2, cstScenarioLearningEpisodes );
    setColumnWidth( 3, cstScenarioTestEpisodes );
    setColumnWidth( 4, cstScenarioProgressColumnWidth );
    setColumnWidth( 5, cstScenarioIterations );

    QStringList labels;
    labels << "Scenario Name" << "# Episodes" << "Learn Every" << "Test every" << "Progress" << "Iterations";
    setHeaderLabels( labels );

    setSelectionMode( QAbstractItemView::ExtendedSelection );

//	setDragEnabled( true );
//	setDropIndicatorShown( true );
//	setDragDropMode( QAbstractItemView::DragDrop );
}

WidgetScenariiList::~WidgetScenariiList()
{
    std::list<WidgetScenarioItem*>::iterator itr;

    for( itr = m_Items.begin(); itr != m_Items.end(); ++itr )
    {
        delete (*itr);
    }
}


void
WidgetScenariiList::clear()
{
    QTreeWidget::clear();
    m_Items.clear();
}


void
WidgetScenariiList::setDisabled( bool f )
{
	QTreeWidget::setDisabled( f );
//	for( std::list<WidgetScenarioItem*>::const_iterator itr = m_Items.begin(); itr != m_Items.end(); ++itr )
//	{
//		(*itr)->setDisabled( f );
//	}
}


void
WidgetScenariiList::addListItem( const QString & _name, unsigned nbEpisode )
{
    WidgetScenarioItem* item = new WidgetScenarioItem( m_Items.size(), _name, this );
    this->addTopLevelItem(item);
    item->setEpisodeCount(nbEpisode);
    connect(item,SIGNAL(scenarioNbrEpisodesChanged(int,int)),this,SIGNAL(scenarioNbrEpisodesChanged(int,int)));

    if (m_Items.size() == 0)
    {
        setCurrentItem( item );
    }
    m_Items.push_back( item );
}
