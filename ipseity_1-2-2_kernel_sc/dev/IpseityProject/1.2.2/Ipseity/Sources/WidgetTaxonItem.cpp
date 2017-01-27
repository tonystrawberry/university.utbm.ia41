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


#include "WidgetTaxonItem.h"

#include "AL/Taxon.h"

#include "IO/ParameterCheckBox.h"
#include "IO/ParameterComboBox.h"


WidgetTaxonItem::WidgetTaxonItem( const QString & taxon_name, uint32 status, QTreeWidget* _parent) :
    QTreeWidgetItem(_parent)
{
    this->setSizeHint(0,QSize(150,30));
    this->setSizeHint(1,QSize(80,30));
    this->setSizeHint(2,QSize(80,30));
    this->setSizeHint(3,QSize(140,30));
    this->setSizeHint(4,QSize(240,30));

    m_TaxonName = new QLabel( taxon_name );

    m_TaxonStatusControllable = new QCheckBox();
    m_TaxonStatusControllable->setEnabled( false );
    m_TaxonStatusControllable->setChecked( status & TS_Controllable );
    connect( m_TaxonStatusControllable, SIGNAL(toggled(bool)), this, SLOT(updateTaxonStatusControllable(bool)) );

    m_TaxonStatusSupervisor = new QCheckBox();
    m_TaxonStatusSupervisor->setEnabled( false );
    m_TaxonStatusSupervisor->setChecked( status & TS_Supervisor );

    Ipseity::String name;

    name = "FlagSharedCS-";
    name += taxon_name.toStdString().c_str();
    m_TaxonStatusSharedCognitiveSystem = new ParameterCheckBox( name.c_str(), "" );
    m_TaxonStatusSharedCognitiveSystem->setEnabled( false );
    m_TaxonStatusSharedCognitiveSystem->setChecked( status & TS_SharedCognitiveSystem );

    name = "CS-";
    name += taxon_name.toStdString().c_str();
    m_CognitiveSystemName = new ParameterComboBox( name.c_str(), "" );
    m_CognitiveSystemName->setFixedWidth( 200 );
    m_CognitiveSystemName->setEnabled( false );
    connect( m_CognitiveSystemName, SIGNAL(currentIndexChanged(int)), this, SLOT(currentNameIndexChanged(int)) );

    if (_parent != NULL)
    {
        this->treeWidget()->setItemWidget( this, 0, m_TaxonName );
        this->treeWidget()->setItemWidget( this, 1, m_TaxonStatusControllable );
        this->treeWidget()->setItemWidget( this, 2, m_TaxonStatusSupervisor );
        this->treeWidget()->setItemWidget( this, 3, m_TaxonStatusSharedCognitiveSystem );
        this->treeWidget()->setItemWidget( this, 4, m_CognitiveSystemName );
    }
}


WidgetTaxonItem::~WidgetTaxonItem()
{
    delete m_TaxonName;
    delete m_TaxonStatusControllable;
    delete m_TaxonStatusSharedCognitiveSystem;
    delete m_CognitiveSystemName;
}


void
WidgetTaxonItem::addCognitiveSystem( const char* cs_name )
{
    m_CognitiveSystemName->setEnabled( true );
    m_TaxonStatusSharedCognitiveSystem->setEnabled( true );
    m_CognitiveSystemName->addItem( QString( cs_name ) );
}


void
WidgetTaxonItem::setCognitiveSystemNameEnabled( bool f )
{
    m_TaxonStatusSharedCognitiveSystem->setEnabled( f );
    m_CognitiveSystemName->setEnabled( f );
}


void
WidgetTaxonItem::toggleShareCognitiveSystem()
{
    m_TaxonStatusSharedCognitiveSystem->setChecked( !m_TaxonStatusSharedCognitiveSystem->isChecked() );
}


void
WidgetTaxonItem::changeIndex(int index)
{
    m_CognitiveSystemName->setCurrentIndex( index );
}


uint32
WidgetTaxonItem::getStatus() const
{
    uint32 status = 0;

    if (m_TaxonStatusControllable->isChecked())
    {
        status |= TS_Controllable;
    }
    if (m_TaxonStatusSupervisor->isChecked())
    {
        status |= TS_Supervisor;
    }
    if (m_TaxonStatusSharedCognitiveSystem->isChecked())
    {
        status |= TS_SharedCognitiveSystem;
    }

    return status;
}


void
WidgetTaxonItem::updateTaxonStatusControllable( bool toggle )
{
    emit updateTaxonStatusControllable( this->treeWidget()->indexOfTopLevelItem(this), toggle );
}


void
WidgetTaxonItem::currentNameIndexChanged( int index )
{
    emit currentNameIndexChanged( this->treeWidget()->indexOfTopLevelItem(this), index );
}
